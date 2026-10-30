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

#include "hook.h"
#include "general/dlhook.h"
#include "logging.h"
#include "GlobalState.h"

#include <string>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/task.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#endif

namespace libtas {

void* link_function(void** function, const char* source, const char* library, const char *version /*= nullptr*/)
{
    /* Test if function is already linked */
    if (*function != nullptr)
        return *function;

    /* First try to link it from the global namespace */
#if defined(__APPLE__) && defined(__MACH__)
    /* On macOS, the regular dlsym path goes through our hook which fails.
     * Use direct Mach-O symbol table parsing to find the symbol. */
    if (library != nullptr) {
        /* Try to resolve in a specific library first */
        *function = resolve_libsymbol(source, library);
        if (*function != nullptr) {
            LOG(LL_DEBUG, LCF_HOOK, "Imported from mach lib %s symbol %s function : %p", library, source, *function);
            if (library)
                add_lib(library);
            return *function;
        }
    }

    /* Fallback to libsystem_c.dylib for standard library functions */
    *function = resolve_native_symbol(source);
    if (*function != nullptr) {
        Dl_info info;
        int res = dladdr(*function, &info);
        if (res != 0) {
            LOG(LL_DEBUG, LCF_HOOK, "Imported symbol %s function : %p (from %s)", source, *function, info.dli_fname);
        }
        else {
            LOG(LL_DEBUG, LCF_HOOK, "Imported symbol %s function : %p", source, *function);
        }
        return *function;
    }
#else
#ifdef __linux__
    if (version)
        *function = dlvsym(RTLD_NEXT, source, version);
    if (*function == nullptr)
#endif
        NATIVECALL(*function = dlsym(RTLD_NEXT, source));

    if (*function != nullptr) {
        /* Gather from which library was the function imported */
        Dl_info info;
        int res = dladdr(*function, &info);
        if (res != 0) {
            LOG(LL_DEBUG, LCF_HOOK, "Imported symbol %s function : %p (from %s)", source, *function, info.dli_fname);
        }
        else {
        LOG(LL_DEBUG, LCF_HOOK, "Imported symbol %s function : %p", source, *function);
        }
        return *function;
    }
#endif

    if (library != nullptr) {

        /* If it did not succeed, try to link using a matching library
         * loaded by the game. (Linux only - macOS uses resolve_libsymbol above) */
#ifndef __APPLE__
        std::string libpath = find_lib(library);

        void* handle;
        if (! libpath.empty()) {

            /* Try to link again using a matching library */
            NATIVECALL(handle = dlopen(libpath.c_str(), RTLD_LAZY));

            if (handle != NULL) {
                NATIVECALL(*function = dlsym(handle, source));

                if (*function != nullptr) {
                    LOG(LL_DEBUG, LCF_HOOK, "Imported from lib %s symbol %s function : %p", libpath.c_str(), source, *function);
                    return *function;
                }
            }
        }

        /* If it did not succeed, try to link using the given library */
        NATIVECALL(handle = dlopen(library, RTLD_LAZY));

        if (handle != NULL) {
            NATIVECALL(*function = dlsym(handle, source));

            if (*function != nullptr) {
                LOG(LL_DEBUG, LCF_HOOK, "Imported from lib %s symbol %s function : %p", library, source, *function);

                /* Add the library to our set of libraries */
                add_lib(library);
                return *function;
            }
        }
#endif
    }

    LOG(LL_ERROR, LCF_HOOK, "Could not import symbol %s", source);

    *function = nullptr;
    return nullptr;
}

}
