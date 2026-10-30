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

#include "winehook.h"

#include "logging.h"
#include "GlobalState.h"
#include "global.h"
#include "general/dlhook.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <string>
#include <vector>
#include <utility>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <unistd.h>

namespace libtas {

namespace {

/* Memory layout of a loaded library */
struct LibraryLayout {
    const char* name;
    uintptr_t start = UINTPTR_MAX;
    uintptr_t end = 0;
    std::vector<std::pair<uintptr_t, uintptr_t>> writable_segments;
    uintptr_t relro_start = 0;
    uintptr_t relro_end = 0;
};

int layout_callback(struct dl_phdr_info *info, size_t, void *data)
{
    LibraryLayout* layout = static_cast<LibraryLayout*>(data);
    if (!info->dlpi_name || !strstr(info->dlpi_name, layout->name))
        return 0;

    for (int i = 0; i < info->dlpi_phnum; i++) {
        const ElfW(Phdr)& phdr = info->dlpi_phdr[i];
        uintptr_t seg_start = info->dlpi_addr + phdr.p_vaddr;
        uintptr_t seg_end = seg_start + phdr.p_memsz;

        if (phdr.p_type == PT_LOAD) {
            if (seg_start < layout->start) layout->start = seg_start;
            if (seg_end > layout->end) layout->end = seg_end;
            if (phdr.p_flags & PF_W)
                layout->writable_segments.emplace_back(seg_start, seg_end);
        }
        else if (phdr.p_type == PT_GNU_RELRO) {
            layout->relro_start = seg_start;
            layout->relro_end = seg_end;
        }
    }
    return 1;
}

bool get_layout(LibraryLayout& layout)
{
    NATIVECALL(dl_iterate_phdr(layout_callback, &layout));
    return layout.end != 0;
}

uintptr_t ntdll_start = 0;
uintptr_t ntdll_end = 0;
uintptr_t x11drv_start = 0;
uintptr_t x11drv_end = 0;

bool wine_game = false;

}

bool wine_should_init(int argc, char** argv)
{
    const char* game;
    NATIVECALL(game = getenv("LIBTAS_WINE_GAME"));
    if (!game)
        return true;

    /* Wine processes are started with the Windows executable as first
     * argument, either as a unix or Windows path */
    const char* exe = "";
    if (argc > 1) {
        exe = argv[1];
        for (const char* c = argv[1]; *c; c++)
            if ((*c == '/') || (*c == '\\'))
                exe = c + 1;
    }

    if (strcasecmp(exe, game) != 0) {
        LOG(LL_INFO, LCF_WINE, "Skipping libtas init in process %s %s", argc > 0 ? argv[0] : "", exe);

        /* Wine reads the display modes and the monitor size in its
         * explorer.exe desktop process, so our XRandR hooks must describe
         * the custom screen resolution there too, or the game is given the
         * size of the real screen */
        const char* screen_size;
        NATIVECALL(screen_size = getenv("LIBTAS_WINE_SCREEN_SIZE"));
        int width, height;
        if (screen_size && (sscanf(screen_size, "%dx%d", &width, &height) == 2)) {
            Global::shared_config.screen_width = width;
            Global::shared_config.screen_height = height;
        }

        /* Don't propagate our library to the children of the other Windows
         * programs that wine starts. Keep it in the processes that lead to
         * the game: unix processes such as the Proton script (env, python),
         * and the steam.exe of Proton, which starts the game. */
        size_t len = strlen(exe);
        bool windows_program = (len > 4) && (strcasecmp(exe + len - 4, ".exe") == 0);
        if (windows_program && (strcasecmp(exe, "steam.exe") != 0))
            unsetenv("LD_PRELOAD");
        return false;
    }

    wine_game = true;
    return true;
}

bool wine_restart_pending()
{
    if (!wine_game || !getenv("LIBTAS_WINE_RESTART"))
        return false;

    LOG(LL_INFO, LCF_WINE, "Skipping libtas init in wine process that restarts the game");
    /* Keep our library for the restarted game process */
    unsetenv("LIBTAS_WINE_RESTART");
    return true;
}

bool isWineGame()
{
    return wine_game;
}

void register_wine_ntdll(const char* path)
{
    LibraryLayout layout;
    layout.name = path;
    if (!get_layout(layout)) {
        LOG(LL_ERROR, LCF_WINE, "Could not find the memory layout of %s", path);
        return;
    }
    ntdll_start = layout.start;
    ntdll_end = layout.end;
}

bool isWineNtdllAddress(const void* addr)
{
    uintptr_t a = reinterpret_cast<uintptr_t>(addr);
    return (a >= ntdll_start) && (a < ntdll_end);
}

void register_wine_x11drv(const char* path)
{
    LibraryLayout layout;
    layout.name = path;
    if (!get_layout(layout)) {
        LOG(LL_ERROR, LCF_WINE, "Could not find the memory layout of %s", path);
        return;
    }
    x11drv_start = layout.start;
    x11drv_end = layout.end;
}

bool isWineX11drvAddress(const void* addr)
{
    uintptr_t a = reinterpret_cast<uintptr_t>(addr);
    return (a >= x11drv_start) && (a < x11drv_end);
}

void* find_wine_symbol(const char* library, const char* name)
{
    std::string libpath = find_lib(library);
    if (libpath.empty())
        return nullptr;

    void* handle;
    NATIVECALL(handle = dlopen(libpath.c_str(), RTLD_LAZY | RTLD_NOLOAD));
    if (!handle)
        return nullptr;

    void* function;
    NATIVECALL(function = dlsym(handle, name));
    return function;
}

void hook_wine_syscall(const char* library, const char* name, void** orig_function, void* my_function)
{
    std::string libpath = find_lib(library);
    if (libpath.empty()) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not find %s path", library);
        return;
    }

    void* handle;
    NATIVECALL(handle = dlopen(libpath.c_str(), RTLD_LAZY | RTLD_NOLOAD));
    if (!handle) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not load %s: %s", library, dlerror());
        return;
    }

    void* function;
    NATIVECALL(function = dlsym(handle, name));
    if (!function) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not find symbol %s in %s", name, library);
        return;
    }

    LibraryLayout layout;
    layout.name = libpath.c_str();
    if (!get_layout(layout)) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not find the memory layout of %s", library);
        return;
    }

    /* The syscall table is an array of function pointers in a data segment,
     * so look for the function address there. */
    std::vector<void**> entries;
    for (const auto& segment : layout.writable_segments) {
        uintptr_t first = (segment.first + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
        for (uintptr_t a = first; a + sizeof(void*) <= segment.second; a += sizeof(void*)) {
            void** entry = reinterpret_cast<void**>(a);
            if (*entry == function)
                entries.push_back(entry);
        }
    }

    /* Proton has other tables with some of these functions. The syscall
     * table is sorted by name, so keep the entry between its neighbors. */
    if (entries.size() > 1) {
        auto symbolName = [](void* addr) -> const char* {
            Dl_info info;
            if (dladdr(addr, &info) && info.dli_sname && (info.dli_saddr == addr))
                return info.dli_sname;
            return nullptr;
        };
        std::vector<void**> sorted_entries;
        for (void** entry : entries) {
            const char* prev = symbolName(entry[-1]);
            const char* next = symbolName(entry[1]);
            if ((prev || next) &&
                (!prev || (strcmp(prev, name) < 0)) &&
                (!next || (strcmp(next, name) > 0)))
                sorted_entries.push_back(entry);
        }
        entries = sorted_entries;
    }

    if (entries.size() != 1) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Found %zu syscall table entries for %s, expected one", entries.size(), name);
        return;
    }

    void** entry = entries[0];
    LOG(LL_DEBUG, LCF_WINE | LCF_HOOK, "Replacing syscall table entry of %s at %p", name, entry);

    uintptr_t page_size = sysconf(_SC_PAGESIZE);
    void* page = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(entry) & ~(page_size - 1));
    bool relro = (reinterpret_cast<uintptr_t>(entry) >= layout.relro_start) &&
                 (reinterpret_cast<uintptr_t>(entry) < layout.relro_end);

    if (relro && mprotect(page, page_size, PROT_READ | PROT_WRITE) != 0) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not make syscall table writable");
        return;
    }

    *orig_function = function;
    *entry = my_function;

    if (relro)
        mprotect(page, page_size, PROT_READ);
}

}
