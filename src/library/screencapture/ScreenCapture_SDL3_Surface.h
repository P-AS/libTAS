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


#ifndef LIBTAS_SCREENCAPTURE_SDL3_SURFACE_H_INCL
#define LIBTAS_SCREENCAPTURE_SDL3_SURFACE_H_INCL

#include "ScreenCapture_Impl.h"

#include <stdint.h>

namespace libtas {

namespace sdl3 {
    struct SDL_Surface;
}

/**
 * @class ScreenCapture_SDL3_Surface
 * @brief SDL3 window surface screen capture implementation.
 *
 * Captures the screen by copying the window surface of SDL3 games that draw
 * with SDL_UpdateWindowSurface().
 */
class ScreenCapture_SDL3_Surface : public ScreenCapture_Impl {

public:
    int init();
    void initScreenSurface();
    void destroyScreenSurface();
    int getPixelFormat();
    int copyScreenToSurface();
    int getPixelsFromSurface(uint8_t **pixels, bool draw);
    int copySurfaceToScreen();
    void restoreScreenState();

private:
    /* Copy of the window surface */
    sdl3::SDL_Surface* screenSurf = nullptr;
};
}

#endif
