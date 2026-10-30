/*
 * Helper to find real dlopen/dlsym on macOS
 *
 * On macOS, _dyld_func_lookup is a private dyld API not available on
 * modern macOS. We try the best available approach.
 */
#include "dyld_func_lookup_helper.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <dlfcn.h>

int dyld_func_lookup_helper(const char *name, void **address) {
    *address = nullptr;
    if (!name)
        return -1;

    /* On macOS with DYLD_INSERT_LIBRARIES, the inserted library's symbols
     * shadow the main executable's dependencies. RTLD_DEFAULT searches the
     * default scope (main executable + linked libraries), which still finds
     * our hook since our library is loaded first.
     *
     * For this reason, we cannot easily get the original dlopen/dlsym from
     * within this helper. The real functions are resolved through lazy
     * binding when the game calls them - the game's lazy binding stubs
     * point to our hooks, which is what we want for interception.
     *
     * The original function pointers (orig::dlopen, orig::dlsym) will be
     * set up by the dlopen_from function (see dlhook.cpp) which is a
     * macOS-specific weak symbol that bypasses our hooks.
     */
    return -1;
}
#else
extern "C" int _dyld_func_lookup(const char *name, void **address);

int dyld_func_lookup_helper(const char *name, void **address) {
    return _dyld_func_lookup(name, address);
}
#endif
