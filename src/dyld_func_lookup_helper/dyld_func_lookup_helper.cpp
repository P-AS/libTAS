#include "dyld_func_lookup_helper.h"

#ifdef __APPLE__
// macOS: _dyld_func_lookup is a private dyld API not available on modern macOS.
// Use a simple stub that returns the symbol via weak linking or nullptr.
int dyld_func_lookup_helper(const char *name, void **address) {
    // On macOS, we can't easily lookup dyld symbols at runtime.
    // The actual dlopen/dlsym will be resolved via the game's library.
    *address = nullptr;
    return -1;
}
#else
extern "C" int _dyld_func_lookup(const char *name, void **address);

int dyld_func_lookup_helper(const char *name, void **address) {
    return _dyld_func_lookup(name, address);
}
#endif
