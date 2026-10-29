/*
    Header for SDL3 weak import stubs.
    
    On macOS, weak symbol definitions for SDL3 functions are provided
    in sdl3_weak_imports.cpp so that &sdl3::fn references in sdldynapi.cpp
    can resolve at compile time. The weak attribute ensures the game's
    SDL3 library symbols override these at link time.
*/

#ifndef LIBTAS_SDL3_WEAK_IMPORTS_H
#define LIBTAS_SDL3_WEAK_IMPORTS_H

#ifdef __APPLE__
#include "../../external/SDL3.h"
#endif

#endif /* LIBTAS_SDL3_WEAK_IMPORTS_H */
