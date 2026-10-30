/*
    Copyright 2015-2026 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dlhook.h"

#include "logging.h"
#include "hook.h"
#include "global.h"
#include "GlobalState.h"
#ifdef __linux__
#include "wine/winehook.h"
#include "wine/wined3d.h"
#include "wine/user32.h"
#include "wine/kernel32.h"
#endif
#include "backtrace.h"
#include "GameHacks.h"
#include "UnityHacks.h"
#include "fileio/SaveFileList.h"
#include "fileio/SaveFile.h"
#include "rendering/vulkanloader.h"
#ifdef __linux__
#include "../external/elfhacks.h"
#endif
#include "../dyld_func_lookup_helper/dyld_func_lookup_helper.h"
#include "../general/dlreal.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <mach/mach.h>
#include <libkern/OSByteOrder.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>

/* Custom strcmp that doesn't go through our hooked strcmp.
 * Only compares ASCII strings byte-by-byte. */
static int native_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b)
            return (*a > *b) ? 1 : -1;
        a++;
        b++;
    }
    return (*a == *b) ? 0 : ((*a > *b) ? 1 : -1);
}

/* Check if a string starts with a given prefix (native version). */
static bool native_strstarts(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix)
            return false;
        s++;
        prefix++;
    }
    return true;
}

/* Helper: parse a Mach-O header and search for a symbol by name.
 * hdr points to a Mach-O image in memory (already loaded).
 * Returns the function address or nullptr if not found.
 * Uses native_strcmp to avoid hook recursion. */
static void *parse_in_memory(const struct mach_header_64 *hdr, const char *name) {
    /* On-disk 64-bit Mach-O header is 32 bytes before load commands:
     * magic(4) cputype(4) cpusubtype(4) filetype(4) ncmds(4) sizeofcmds(4)
     * flags(4) flags2(4) = 32 bytes. Use mach_header_64 size. */
    struct load_command *lc = reinterpret_cast<struct load_command*>(
        const_cast<unsigned char*>(
            reinterpret_cast<const unsigned char*>(hdr) + sizeof(struct mach_header_64)));
    struct symtab_command *symtab = nullptr;
    uint32_t nsects = 0;
    struct section_64 sects[128];

    for (uint32_t j = 0; j < hdr->ncmds; j++) {
        if (lc->cmd == LC_SYMTAB && !symtab)
            symtab = reinterpret_cast<struct symtab_command*>(lc);
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *seg =
                reinterpret_cast<struct segment_command_64*>(lc);
            struct section_64 *sects_start =
                reinterpret_cast<struct section_64*>(
                    reinterpret_cast<unsigned char*>(seg) +
                    sizeof(struct segment_command_64));
            if (nsects + seg->nsects <= 128) {
                memcpy(&sects[nsects], sects_start,
                       sizeof(struct section_64) * seg->nsects);
                nsects += seg->nsects;
            }
        }
        lc = reinterpret_cast<struct load_command*>(
            const_cast<unsigned char*>(
                reinterpret_cast<const unsigned char*>(lc) + lc->cmdsize));
    }

    if (!symtab)
        return nullptr;

    struct nlist_64 *syms = reinterpret_cast<struct nlist_64*>(
        reinterpret_cast<uintptr_t>(hdr) + symtab->symoff);
    char *strs = reinterpret_cast<char*>(
        reinterpret_cast<uintptr_t>(hdr) + symtab->stroff);

    for (uint32_t j = 0; j < symtab->nsyms; j++) {
        if (syms[j].n_un.n_strx == 0)
            continue;
        const char *sname = strs + syms[j].n_un.n_strx;
        if (native_strcmp(sname, name) != 0)
            continue;

        uint8_t type = syms[j].n_type & N_TYPE;
        uint8_t nsect = syms[j].n_sect;
        if (type == N_SECT && nsect > 0 && nsect <= nsects) {
            /* section.addr is already the virtual address;
             * n_value is the offset within the section. */
            return (void*)(sects[nsect - 1].addr + syms[j].n_value);
        }
        if (type == N_ABS && syms[j].n_value != 0)
            return (void*)syms[j].n_value;
    }
    return nullptr;
}

/* Helper: open a Mach-O file from disk using raw syscalls to bypass
 * the hooked open(). Returns the Mach-O symbol address or nullptr.
 * section.addr in the Mach-O is already an absolute virtual address. */
static void *find_symbol_in_file(const char *filepath, const char *name) {
    extern int syscall(int, ...);
    int fd = syscall(SYS_open, filepath, O_RDONLY);
    if (fd < 0)
        return nullptr;

    struct stat st;
    memset(&st, 0, sizeof(st));
    if (syscall(SYS_fstat64, fd, &st) != 0) {
        syscall(SYS_close, fd);
        return nullptr;
    }

    void *map = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    syscall(SYS_close, fd);
    if (map == MAP_FAILED)
        return nullptr;

    const struct mach_header_64 *hdr =
        reinterpret_cast<const struct mach_header_64*>(map);

    /* Handle fat binaries: the fat header is big-endian regardless of
     * host byte order. Use OSSwapBigToHostInt32 to read all fields. */
    struct fat_header *fh = reinterpret_cast<struct fat_header*>(map);
    uint32_t magic = OSSwapBigToHostInt32(fh->magic);
    if (magic == FAT_MAGIC_64 || magic == FAT_MAGIC) {
        uint32_t nfat_arch = OSSwapBigToHostInt32(fh->nfat_arch);
        if (nfat_arch > 0 && nfat_arch < 128) {
            struct fat_arch *archs_raw = reinterpret_cast<struct fat_arch*>(
                reinterpret_cast<unsigned char*>(fh) + sizeof(struct fat_header));

            int best_idx = -1;
            uint32_t best_align = UINT32_MAX;
            uint64_t best_offset = 0;

            for (uint32_t i = 0; i < nfat_arch; i++) {
                uint32_t cputype = OSSwapBigToHostInt32(archs_raw[i].cputype);
                uint32_t align = archs_raw[i].align;

                if (cputype == CPU_TYPE_ARM64 && align < best_align) {
                    best_align = align;
                    best_idx = static_cast<int>(i);
                    best_offset = OSSwapBigToHostInt32(archs_raw[i].offset);
                }
            }

            if (best_idx >= 0) {
                unsigned char *slice = reinterpret_cast<unsigned char*>(map) + best_offset;
                const struct mach_header_64 *slice_hdr =
                    reinterpret_cast<const struct mach_header_64*>(slice);

                /* Verify the slice is a valid Mach-O 64-bit header
                 * (slice magic is in native byte order) */
                if (slice_hdr->magic == MH_MAGIC_64) {
                    void *sym = parse_in_memory(slice_hdr, name);
                    munmap(map, st.st_size);
                    return sym;
                }
            }
        }
    }
    else if (hdr->magic == MH_MAGIC_64) {
        void *sym = parse_in_memory(hdr, name);
        munmap(map, st.st_size);
        return sym;
    }

    munmap(map, st.st_size);
    return nullptr;
}

/* Helper: try to find a symbol by resolving library paths.
 * On macOS, library names passed to link_function may be bare names
 * like "pthread" — we try multiple known paths. */
static void *resolve_path_symbol(const char *name, const char *hint) {
    const char *paths[] = {
        hint ? hint : "/usr/lib/system/libsystem_pthread.dylib",
        "/usr/lib/system/libsystem_pthread.dylib",
        "/usr/lib/system/libsystem_platform.dylib",
        "/usr/lib/system/libsystem_c.dylib",
        "/usr/lib/system/libsystem_kernel.dylib",
        "/usr/lib/libSystem.B.dylib",
        nullptr
    };

    for (int i = 0; paths[i] != nullptr; i++) {
        void *sym = find_symbol_in_file(paths[i], name);
        if (sym)
            return sym;
    }
    /* Also try with leading underscore (macOS C naming convention).
     * C symbols on macOS use a leading underscore in Mach-O. */
    if (name[0] != '_') {
        char underscored[256];
        underscored[0] = '_';
        size_t len = strlen(name);
        if (len < sizeof(underscored) - 1) {
            memcpy(underscored + 1, name, len + 1);
            for (int i = 0; paths[i] != nullptr; i++) {
                void *sym = find_symbol_in_file(paths[i], underscored);
                if (sym)
                    return sym;
            }
        }
    }
    return nullptr;
}

#endif

#include <cstring>
#include <set>
#include <sys/stat.h>
#ifdef __APPLE__
#include <dlfcn.h>
#endif

namespace libtas {

/* Set of libraries that are loaded by the game using dlopen */
/* It is stored static inside a function, so that it will be created on first
 * use. Indeed, it can be called very early in the program execution. If made
 * file-static, it may not be initialized in time.
 */

static std::set<std::string>& get_lib_set() {
    static std::set<std::string> library_set;
    return library_set;
}

std::string find_lib(const char* library)
{
    std::set<std::string>& library_set = get_lib_set();
    for (auto const& itr : library_set)
        if (itr.find(library) != std::string::npos)
            return itr;

    std::string emptystring;
    return emptystring;
}

void add_lib(const char* library)
{
    if (library) {
        std::set<std::string>& library_set = get_lib_set();
        library_set.insert(std::string(library));
    }
}

DEFINE_ORIG_POINTER(dlopen)
DEFINE_ORIG_POINTER(dlsym)

/* Recursion guard: prevents infinite loops when symbol resolution
 * needs to open files (which goes through the hooked open wrapper). */
static thread_local int g_resolve_depth = 0;

/* Flag: set to false during libTAS init, true after init completes.
 * When false, hooks return the wrapped version directly without
 * trying to resolve orig_* pointers. */
thread_local bool hooks_enabled = false;

/* Enable hooks. Called at the end of init(), after dyld is ready. */
static void enable_hooks(void) __attribute__((constructor(250)));
static void enable_hooks(void) {
    hooks_enabled = true;
}

#ifdef __linux__
/* Declare internal implementation-dependent dlsym function.
 * It is declared as weak, so we have a fallback if not present */
extern "C" void *_dl_sym(void *, const char *, void *) __attribute__((weak));
#endif

/* Find the address of dlopen and dlsym symbols */
static void get_dlfct_symbols()
{
#ifdef __linux__
    /* To access the real dl functions, we use the fact that dlsym
     * calls internally _dl_sym. */
    if (_dl_sym) {
        orig::dlopen = reinterpret_cast<decltype(orig::dlopen)>(_dl_sym(RTLD_NEXT, "dlopen", reinterpret_cast<void*>(dlopen)));
        orig::dlsym = reinterpret_cast<decltype(orig::dlsym)>(_dl_sym(RTLD_NEXT, "dlsym", reinterpret_cast<void*>(dlsym)));
    }
    else {
        /* If _dl_sym is not available (such as with glibc >= 2.34), find the
         * symbols manually by parsing the loaded library */
        /* Code taken from MangoHud: <https://github.com/flightlessmango/MangoHud> */
        const char* libs[] = {
#if defined(__GLIBC__)
            "*libdl.so*",
#endif
            "*libc.so*",
            "*libc.*.so*",
        };

        for (size_t i = 0; i < sizeof(libs) / sizeof(*libs); i++)
        {
            eh_obj_t libdl;
            int ret = eh_find_obj(&libdl, libs[i]);
            if (ret)
                continue;

            eh_find_sym(&libdl, "dlopen", (void **) &orig::dlopen);
            eh_find_sym(&libdl, "dlsym", (void **) &orig::dlsym);
            eh_destroy_obj(&libdl);

            if (orig::dlopen && orig::dlsym)
                break;
            orig::dlopen = nullptr;
            orig::dlsym = nullptr;
        }
    }
    if (!orig::dlopen || !orig::dlsym)
    {
        LOG(LL_ERROR, LCF_HOOK, "Could not get dl function symbols");
        exit(1);
    }
#elif defined(__APPLE__) && defined(__MACH__)
    /* On macOS, dlopen_from is a weak function from the system dyld that
     * acts like dlopen but takes an extra caller address argument. It
     * bypasses the inserted library scope, so it calls the real system
     * dlopen, not our hook. We just leave orig::dlopen null and handle
     * it directly in the dlopen hook. */
    orig::dlopen = nullptr;
    orig::dlsym = nullptr;
#endif
}

#if defined(__APPLE__) && defined(__MACH__)
/* dlopen_from is like dlopen, except it takes an extra argument specifying the "true" caller address.
 * it is marked as weak so that libTAS will still work if it's not present. */
extern "C" void *dlopen_from(const char *file, int mode, void *callerAddr) __attribute__((weak));
#define LIBTAS_NO_IPA
#else
#define LIBTAS_NO_IPA __attribute__((noipa))
#endif

/* Helper for macOS dlsym when orig::dlsym is not available.
 * Uses dlopen_from to get handles and find_sym for libTAS functions. */
static void *find_sym_native(void *handle, const char *name) {
#ifdef __APPLE__
    if (handle == RTLD_DEFAULT) {
        /* Try Vulkan procs first (idempotent check). */
        void *addr = find_sym(name, handle);
        if (addr)
            return addr;
        /* Fall back to native symbol resolution. */
        void *sym = resolve_native_symbol(name);
        if (sym)
            return sym;
        /* Also try common system libraries. */
        sym = resolve_libsymbol(name, nullptr);
        if (sym)
            return sym;
        return nullptr;
    }
    /* Non-RTLD_DEFAULT handle — it's the Mach-O header pointer
     * returned by dlopen_from. Try to match it against loaded
     * dyld images and parse that image's symbol table. */
    {
        uint32_t count = _dyld_image_count();
        for (uint32_t i = 0; i < count; i++) {
            const struct mach_header *raw_hdr =
                _dyld_get_image_header(i);
            const struct mach_header_64 *hdr =
                reinterpret_cast<const struct mach_header_64*>(raw_hdr);
            if (hdr && hdr->magic == MH_MAGIC_64 &&
                hdr == reinterpret_cast<const struct mach_header_64*>(handle)) {
                void *sym = parse_in_memory(hdr, name);
                if (sym)
                    return sym;
                break; /* Found the image but symbol not in it. */
            }
        }
    }
    return nullptr;
#else
    (void)handle;
    (void)name;
    return nullptr;
#endif
}

/* macOS: resolve a symbol by name using file-based Mach-O parsing.
 * For already-loaded libraries, dlopen_from(RTLD_NOLOAD) gives the
 * runtime handle with correct addresses. Falls back to mmap for
 * un-loaded libraries. */
#ifdef __APPLE__
void *resolve_native_symbol(const char *name) {
    g_resolve_depth++;
    void *sym = nullptr;
    if (g_resolve_depth == 1) {
        /* Try file-based lookup in common system libraries first. */
        sym = resolve_path_symbol(name, nullptr);
        if (!sym) {
            /* Fallback: scan backwards from return address for a single image.
             * This works for symbols in the caller's own library. */
            void *caller = __builtin_return_address(0);
            if (caller) {
                unsigned char *addr = reinterpret_cast<unsigned char*>(caller);
                for (int i = 0; i < 4096 && addr > reinterpret_cast<unsigned char*>(0x100000000); i++) {
                    struct mach_header_64 *hdr = reinterpret_cast<struct mach_header_64*>(addr - i);
                    if (hdr->magic == MH_MAGIC_64) {
                        sym = parse_in_memory(hdr, name);
                        if (sym) break;
                        break;
                    }
                }
            }
        }
    }
    g_resolve_depth--;
    return sym;
}

/* macOS: resolve a symbol in a library loaded by the game.
 * Uses dlopen_from(RTLD_NOLOAD) to get the runtime handle, which
 * provides the correct virtual address for section parsing. */
void *resolve_libsymbol(const char *name, const char *library_hint) {
    g_resolve_depth++;
    void *sym = nullptr;
    if (g_resolve_depth == 1) {
        /* Try dlopen_from(RTLD_NOLOAD) first — this returns the already
         * loaded library handle with correct runtime addresses. */
        if (dlopen_from) {
            const char *paths[] = {
                library_hint ? library_hint : "/usr/lib/system/libsystem_pthread.dylib",
                "/usr/lib/system/libsystem_pthread.dylib",
                "/usr/lib/system/libsystem_platform.dylib",
                "/usr/lib/system/libsystem_c.dylib",
                "/usr/lib/system/libsystem_kernel.dylib",
                "/usr/lib/libSystem.B.dylib",
                nullptr
            };

            for (int i = 0; paths[i] != nullptr; i++) {
                void *handle = dlopen_from(paths[i], RTLD_NOLOAD, nullptr);
                if (!handle)
                    continue;

                /* Parse the Mach-O at its runtime address — section.addr
                 * is already the correct virtual address. */
                const struct mach_header_64 *hdr =
                    reinterpret_cast<const struct mach_header_64*>(handle);
                if (hdr->magic == MH_MAGIC_64) {
                    sym = parse_in_memory(hdr, name);
                    if (sym)
                        break;
                }
            }
        }
        /* Fallback to file-based lookup for un-loaded libraries. */
        if (!sym) {
            sym = resolve_path_symbol(name, library_hint);
        }
    }
    g_resolve_depth--;
    return sym;
}
#endif  /* __APPLE__ */

LIBTAS_NO_IPA void *dlopen(const char *file, int mode) __THROW {
    void *const callerAddr = __builtin_extract_return_addr(__builtin_return_address(0));

    if (!orig::dlopen) {
        get_dlfct_symbols();
    }

    if (GlobalState::isNative()) {
#ifdef __APPLE__
        /* Use dlopen_from which bypasses our dlopen hook */
        if (dlopen_from)
            return dlopen_from(file, mode, reinterpret_cast<void*>(callerAddr));
        return nullptr;
#else
        return orig::dlopen(file, mode);
#endif
    }

    /* Non-native: game calling dlopen. On macOS, dlopen_from bypasses the hook. */
#ifdef __APPLE__
    if (dlopen_from) {
        return dlopen_from(file, mode, reinterpret_cast<void*>(callerAddr));
    }
#endif
    return orig::dlopen(file, mode);

    /* Block access to pulseaudio so that games will default to ALSA.
     * For some Unity games using libAkSoundEngine.so it will crash, but we
     * hooked to library functions to enforce ALSA, so we can safely allow
     * opening libpulse */
    if (file != nullptr && std::strstr(file, "libpulse") != nullptr && !(Global::game_info.audio & GameInfo::AKAUDIO)) {
        LOG(LL_DEBUG, LCF_HOOK, "%s blocked access to library %s", __func__, file);
        return nullptr;
    }

    if (file != nullptr && std::strstr(file, "ScreenSelector.so") != nullptr) {
        LOG(LL_DEBUG, LCF_HOOK, "%s blocked access to library %s", __func__, file);
        return nullptr;
    }

    LOGTRACE(LCF_HOOK, "%s call with file %s and flags 0x%x", __func__, (file!=nullptr)?file:"<NULL>", mode);
    void *result = nullptr;

#ifdef RTLD_DEEPBIND
    if (mode & RTLD_DEEPBIND) {
        LOG(LL_DEBUG, LCF_HOOK, "   removing RTLD_DEEPBIND flag");
        mode &= ~RTLD_DEEPBIND;
    }
#endif

#if defined(__APPLE__) && defined(__MACH__)
    if (dlopen_from) {
        result = dlopen_from(file, mode, reinterpret_cast<void*>(callerAddr));

        if (result != nullptr) {
            add_lib(file);
        }
    } else
#endif
    {
#ifdef __linux__
        if (file != nullptr && file[0] != '\0' && std::strchr(file, '/') == nullptr) {
            /* Path should be searched using search paths, so let's
             * manually check the paths in the correct order...
             */
            Dl_info info;
            if (dladdr(callerAddr, &info)) {
                /* Get the dynamic library name of our caller */
                const char *dlname = info.dli_fname;
                {
                    struct stat dlstat, exestat;
                    if (stat(dlname, &dlstat) == 0 &&
                        stat("/proc/self/exe", &exestat) == 0 &&
                        dlstat.st_dev == exestat.st_dev &&
                        dlstat.st_ino == exestat.st_ino) {
                        /* Unless being called from the main executable */
                        dlname = nullptr;
                    }
                }

                /* Open object of caller */
                void *caller = orig::dlopen(dlname, RTLD_LAZY | RTLD_NOLOAD);
                if (caller != nullptr) {
                    Dl_serinfo size;
                    if (dlinfo(caller, RTLD_DI_SERINFOSIZE, &size) == 0) {
                        auto *paths = reinterpret_cast<Dl_serinfo *>(new char[size.dls_size]);
                        *paths = size;

                        /* Get ordered list of search paths for this object */
                        if (dlinfo(caller, RTLD_DI_SERINFO, paths) == 0) {
                            for (unsigned i = 0; i != paths->dls_cnt; ++i) {
                                const char *name = paths->dls_serpath[i].dls_name;
                                /* Probably can't happen, just being safe... */
                                if (name == nullptr || name[0] == '\0')
                                    continue;
                                std::string path(name);
                                /* Note that this guaranteed / prevents
                                 * recursive search path lookup
                                 */
                                if (path.back() != '/')
                                    path += '/';
                                path += file;

                                result = orig::dlopen(path.c_str(), mode);
                                if (result != nullptr) {
                                    LOG(LL_DEBUG, LCF_HOOK, "   Found at %s", name);
                                    add_lib(path.c_str());
                                    break;
                                }
                            }
                        }

                        delete [] reinterpret_cast<char *>(paths);
                    }

                    dlclose(caller);
                }
            }
        }
#endif

        if (result == nullptr) {
            /* Path is empty (referring to the main program), relative to
             * the cwd, absolute, or failed search path lookup above, so
             * try looking it up normally.
             */
            result = orig::dlopen(file, mode);

            if (result != nullptr)
                add_lib(file);
        }
    }

#ifdef __linux__
    if (result && file && std::strstr(file, "wined3d.dll.so") != nullptr) {
        /* Hook wine wined3d functions */
        hook_wined3d();
    }

    if (result && file && std::strstr(file, "user32.dll.so") != nullptr) {
        /* Hook wine user32 functions */
        hook_user32();
    }

    if (result && file && std::strstr(file, "kernel32.dll.so") != nullptr) {
        /* Hook wine kernel32 functions */
        hook_kernel32();
    }
#endif

    if (result && file && std::strstr(file, "libcoreclr.so") != nullptr)
        GameHacks::setCoreclr();

    if (!result) {
        /* Maybe the file is a savefile, so it is missing in the actual path.
         * There is no function to load a library from a file descriptor, so
         * we write back the file and remove it after. */
        const SaveFile* savefile = SaveFileList::getSaveFile(file);
        if (savefile) {
            LOG(LL_DEBUG, LCF_HOOK, "Write back savefile %s to disk", file);
            savefile->saveOnDisk();
            result = orig::dlopen(file, mode);
            savefile->removeFromDisk();
        }
    }

    return result;
}

/* Recursion guard for find_sym: prevents infinite loop when getenv
 * hook calls link_function which calls dlsym which calls find_sym. */
static thread_local const char* finding_sym = nullptr;

void *find_sym(const char *name, void *handle) {

    /* First try one of our own Vulkan functions, because they cannot be found using dlsym. */
    void *addr = vk_find_my_proc(name);
    if (addr)
        return addr;

    /* Recursion guard: if we're already resolving this exact symbol,
     * return nullptr to break the cycle. The symbol will be found via
     * link_function's other resolution paths (e.g. _dl_sym on Linux,
     * or loaded library search on macOS). */
    if (finding_sym == name) {
        return nullptr;
    }
    finding_sym = name;

    GlobalNative gn;

    /* Use direct environ block access (bypasses the getenv hook).
     * On macOS, getenv() calls link_function which calls dlsym which
     * calls find_sym again, creating infinite recursion. */
    const char* libtaspath = getenv_direct_call("LIBTAS_LIBRARY_PATH");
    if (libtaspath == nullptr) {
        LOG(LL_ERROR, LCF_HOOK, "   LIBTAS_LIBRARY_PATH is not set, guess libtas.so path");
        finding_sym = nullptr;
        return nullptr;
    }
    std::string libtasstr(libtaspath);

    void *libtaslib = dlopen(libtaspath, RTLD_LAZY | RTLD_NOLOAD);
    if (libtaslib == nullptr) {
        LOG(LL_ERROR, LCF_HOOK, "   Could not find already loaded libtas.so!");
        finding_sym = nullptr;
        return nullptr;
    }

    dlerror(); // Clear pending errors

#ifdef __APPLE__
    /* On macOS, orig::dlsym is null (we use dlopen_from for dlopen).
     * We scan loaded dyld images to find symbols.
     * handle == nullptr means RTLD_DEFAULT — scan all libraries.
     * handle != nullptr means a specific library — skip libTAS
     * symbols and let find_sym_native() handle library-scoped lookup. */
    if (!orig::dlsym) {
        uint32_t count = _dyld_image_count();
        bool found = false;
        for (uint32_t i = 0; i < count; i++) {
            const struct mach_header *raw_hdr =
                _dyld_get_image_header(i);
            const struct mach_header_64 *hdr =
                reinterpret_cast<const struct mach_header_64*>(raw_hdr);
            if (!hdr || hdr->magic != MH_MAGIC_64)
                continue;
            void *sym = parse_in_memory(hdr, name);
            if (sym) {
                found = true;
                /* For RTLD_DEFAULT (handle == nullptr):
                 * In native mode, return any symbol (library or libTAS).
                 * In Wine mode, only return libTAS symbols.
                 * For specific library handles (handle != nullptr):
                 * skip libTAS symbols — let find_sym_native() handle it. */
                if (handle == nullptr) {
                    if (GlobalState::isNative()) {
                        finding_sym = nullptr;
                        return sym;
                    }
                    const char *imgname = _dyld_get_image_name(i);
                    if (imgname && strstr(imgname, "libtas.so") != nullptr) {
                        finding_sym = nullptr;
                        return sym;
                    }
                }
            }
        }
        /* Not found (or we skipped libTAS for specific handle). */
        if (!found && handle == nullptr) {
            finding_sym = nullptr;
            return nullptr;
        }
        /* For specific library handle, fall through to find_sym_native().
         * We set addr = nullptr so dlsym() falls through to find_sym_native. */
        addr = nullptr;
    }
    else
#endif
    {
        /* Original path for Linux and macOS fallback. */
        std::string libpath;
        addr = orig::dlsym(libtaslib, name);
        if (dlerror() == nullptr) {
            Dl_info info;
            int res = dladdr(addr, &info);
            if (res != 0) {
                libpath = info.dli_fname;
                bool fromLibtas = false;
                fromLibtas = libpath.length() >= libtasstr.length() &&
                    libpath.compare(libpath.length()-libtasstr.length(), libtasstr.length(), libtasstr) == 0;
                if (!fromLibtas) {
                    finding_sym = nullptr;
                    return nullptr;
                }
            }
        }
        else {
            libpath = "";
        }
    }

    finding_sym = nullptr;
    return addr;
}

void *dlsym(void *handle, const char *name) __THROW {
    if (!orig::dlsym) {
        get_dlfct_symbols();
    }

    /* dlsym() does some work besides the actual function, and that may call
     * other hooked functions that themselves call dlsym(). If we detect a recursion,
     * we switch to the pure dlsym() function like above.
     * This is especially the case for jemalloc which calls a bunch of functions,
     * and dlsym allocates buffers for dlerror().
     */
    static thread_local int recurs_count = 0;
    bool safe = (recurs_count > 0);

    recurs_count++;

    if (GlobalState::isNative()) {
        void* ret;
        if (safe) {
#ifdef __linux__
            if (_dl_sym)
                ret = _dl_sym(handle, name, reinterpret_cast<void*>(dlsym));
            else
                ret = orig::dlsym(handle, name);
#elif defined(__APPLE__) && defined(__MACH__)
            /* On macOS, get handle via dlopen_from and call find_sym.
             * Since we can't call the real dlsym, we rely on find_sym
             * to provide the libTAS-defined functions. */
            ret = find_sym(name, handle);
#else
            ret = orig::dlsym(handle, name);
#endif
        }
        else {
#ifdef __APPLE__
            ret = find_sym(name, handle);
#else
            ret = orig::dlsym(handle, name);
#endif
        }
        recurs_count--;
        return ret;
    }

    LOGTRACE(LCF_HOOK, "%s call with function %s %s", __func__, name, safe?"(safe)":"");

    /* Special cases when dlsym is called with dl* functions (yes, it happens...). */
    if (strcmp(name, "dlopen") == 0) {
        recurs_count--;
        return reinterpret_cast<void*>(dlopen);
    }
    if (strcmp(name, "dlsym") == 0) {
        recurs_count--;        
        return reinterpret_cast<void*>(dlsym);
    }

    /* Special case for RTLD_NEXT. The order of loaded libraries are affected
     * by us preloading libtas.so, so if the game relies on the order of
     * libraries by calling dlsym with RTLD_NEXT, it may return the wrong
     * function and an infinite call.

     * We are handling each case here. */
    if (handle == RTLD_NEXT) {
#ifdef __APPLE__
        /* On macOS, RTLD_NEXT doesn't work the same way as on Linux.
         * For now, just pass through to find_sym. */
        LOG(LL_WARN, LCF_HOOK, "   dlsym called with RTLD_NEXT for symbol %s! (skipped on macOS)", name);
#else
        GlobalNative gn;

        /* Chrome */
        if ((strcmp(name, "localtime") == 0) ||
            (strcmp(name, "localtime64") == 0) ||
            (strcmp(name, "localtime_r") == 0) ||
            (strcmp(name, "localtime64_r") == 0)) {
            void* libc_handle = dlopen("libc.so.6", RTLD_LAZY);
            void* ret = orig::dlsym(libc_handle, name);
            recurs_count--;
            return ret;
        }
        else {
            LOG(LL_WARN, LCF_HOOK, "   dlsym called with RTLD_NEXT for symbol %s!", name);
        }
#endif
    }

    /* Detect a Unity game when it loads specific Unity functions */
    if (std::strstr(name, "mono_unity_") != nullptr)
        UnityHacks::setUnity();

    /* FIXME: This design is not good enough.
     * This idea is to link to our defined function when there is one, instead
     * of the function inside the library that the game wants to load.
     * However, there is a problem in this, encountered in Towerfall:
     * This game bundles with libpng 1.5.10, and load this library dynamically.
     * Also, the ffmpeg libraries that we use are usually compiled with png
     * support, which means that we statically link to our libpng (1.6.28 for me)
     * Thus, calling `dlsym(RTLD_DEFAULT, "png_xxx")` will return the function
     * from our installed libpng library, which has no reason to be of the same
     * version as the one bundled with the game.
     *
     * One solution would be to check if the symbol name is defined strictly
     * in our library, excluding any shared library dependency.
     * AFAIK, dlopen and dlsym always include all dependencies. Maybe by doing
     * some arithmetic pointer shenanigan?
     */

    void *addr = find_sym(name, handle);

#ifdef __APPLE__
    /* On macOS, orig::dlsym may be null (we use dlopen_from for dlopen).
     * The find_sym function handles Vulkan proc interception.
     * For other symbols, we pass through by using the library handle directly. */
    if (addr == nullptr) {
        addr = find_sym_native(handle, name);
    }
#else
    void *orig_addr = orig::dlsym(handle, name);
    if (addr == nullptr) {
        addr = orig_addr;
    }
#endif
    /* We store the original pointer now, in case we may not be able to get it later */
#ifdef __APPLE__
    if (addr) {
        vk_store_proc(name, addr);
    }
#else
    if (orig_addr) {
        vk_store_proc(name, orig_addr);
    }
#endif

#ifdef __linux__
    if (0 == strcmp(name, "__wine_process_init")) {
        /* Hook wine LdrGetProcedureAddress function */
        hook_ntdll();
    }
#endif

    recurs_count--;
    return addr;
}

}
