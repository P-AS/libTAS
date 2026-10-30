/*
 * Helper to read environment variables on macOS without triggering the hook.
 *
 * Uses _NSGetEnviron() which is a stable public macOS API to read the
 * environment block directly, bypassing any getenv hook.
 */

#include "dlreal.h"

#ifdef __APPLE__
#include <crt_externs.h>
#include <string.h>
#endif

/* Read an environment variable directly from the environ block,
 * bypassing any getenv hook. Uses _NSGetEnviron() which is a
 * stable, public macOS API (not a private dyld symbol). */
static const char* getenv_direct(const char *name) {
#ifdef __APPLE__
    char ***envp = _NSGetEnviron();
    if (!envp || !*envp)
        return nullptr;
    char **env = *envp;
    size_t namelen = strlen(name);
    for (char **ep = env; *ep != nullptr; ep++) {
        if (strncmp(*ep, name, namelen) == 0 && (*ep)[namelen] == '=')
            return *ep + namelen + 1;
    }
#else
    (void)name;
#endif
    return nullptr;
}

/* Call getenv_direct: read environment variable without any hook. */
const char* getenv_direct_call(const char *name) {
    return getenv_direct(name);
}
