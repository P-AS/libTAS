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


#include "ScreenCapture.h"
#include "ScreenCapture_SDL3_Surface.h"
#include "hook.h"
#include "logging.h"
#include "global.h"
#include "GlobalState.h"
#include "sdl/sdlwindows.h" // sdl::gameSDLWindow
#include "sdl/sdldynapi.h"

#include "../external/SDL3.h"

namespace libtas {

int ScreenCapture_SDL3_Surface::init()
{
    if (ScreenCapture_Impl::init() < 0)
        return -1;

    ORIG_SDL3_CALL(SDL_GetWindowSize, (sdl::gameSDLWindow, &width, &height));
    sdl3::SDL_PixelFormat sdlpixfmt = ORIG_SDL3_CALL(SDL_GetWindowPixelFormat, (sdl::gameSDLWindow));
    pixelSize = sdlpixfmt & 0xFF;

    return ScreenCapture_Impl::postInit();
}

void ScreenCapture_SDL3_Surface::initScreenSurface()
{
    sdl3::SDL_Surface *surf = ORIG_SDL3_CALL(SDL_GetWindowSurface, (sdl::gameSDLWindow));
    if (!surf) {
        LOG(LL_ERROR, LCF_WINDOW | LCF_SDL, "SDL_GetWindowSurface failed: %s", ORIG_SDL3_CALL(SDL_GetError, ()));
        return;
    }
    screenSurf = ORIG_SDL3_CALL(SDL_ConvertSurface, (surf, surf->format));
}

void ScreenCapture_SDL3_Surface::destroyScreenSurface()
{
    if (screenSurf) {
        ORIG_SDL3_CALL(SDL_DestroySurface, (screenSurf));
        screenSurf = nullptr;
    }
}

int ScreenCapture_SDL3_Surface::getPixelFormat()
{
    sdl3::SDL_PixelFormat sdlpixfmt = ORIG_SDL3_CALL(SDL_GetWindowPixelFormat, (sdl::gameSDLWindow));
    switch (sdlpixfmt) {
        case sdl3::SDL_PIXELFORMAT_RGBA8888:
            return ScreenCapture::PIXELFORMAT_BGRA8;
        case sdl3::SDL_PIXELFORMAT_BGRA8888:
            return ScreenCapture::PIXELFORMAT_RGBA8;
        case sdl3::SDL_PIXELFORMAT_ARGB8888:
            return ScreenCapture::PIXELFORMAT_ABGR8;
        case sdl3::SDL_PIXELFORMAT_ABGR8888:
            return ScreenCapture::PIXELFORMAT_ARGB8;
        case sdl3::SDL_PIXELFORMAT_XRGB8888:
            return ScreenCapture::PIXELFORMAT_BGRX8;
        case sdl3::SDL_PIXELFORMAT_RGBX8888:
            return ScreenCapture::PIXELFORMAT_XBGR8;
        case sdl3::SDL_PIXELFORMAT_XBGR8888:
            return ScreenCapture::PIXELFORMAT_RGBX8;
        case sdl3::SDL_PIXELFORMAT_BGRX8888:
            return ScreenCapture::PIXELFORMAT_XRGB8;
        case sdl3::SDL_PIXELFORMAT_RGB24:
            return ScreenCapture::PIXELFORMAT_BGR24;
        case sdl3::SDL_PIXELFORMAT_BGR24:
            return ScreenCapture::PIXELFORMAT_RGB24;
        default:
            LOG(LL_ERROR, LCF_DUMP | LCF_SDL, "  Unsupported pixel format %d", sdlpixfmt);
    }

    return ScreenCapture::PIXELFORMAT_UNKNOWN;
}

int ScreenCapture_SDL3_Surface::copyScreenToSurface()
{
    GlobalNative gn;

    sdl3::SDL_Surface* surf = ORIG_SDL3_CALL(SDL_GetWindowSurface, (sdl::gameSDLWindow));
    if (!surf || !screenSurf)
        return -1;

    /* Checking for a size modification */
    if ((surf->w != width) || (surf->h != height)) {
        LOG(LL_ERROR, LCF_DUMP, "Window coords have changed (%d,%d) -> (%d,%d)", width, height, surf->w, surf->h);
        return -1;
    }

    ORIG_SDL3_CALL(SDL_BlitSurface, (surf, nullptr, screenSurf, nullptr));

    return size;
}

int ScreenCapture_SDL3_Surface::getPixelsFromSurface(uint8_t **pixels, bool draw)
{
    if (pixels) {
        *pixels = winpixels.data();
    }

    if (!draw || !screenSurf)
        return size;

    /* We must lock the surface before accessing the raw pixels */
    bool mustlock = screenSurf->flags & sdl3::SDL_SURFACE_LOCK_NEEDED;
    if (mustlock && !ORIG_SDL3_CALL(SDL_LockSurface, (screenSurf))) {
        LOG(LL_ERROR, LCF_DUMP | LCF_SDL, "Could not lock SDL surface: %s", ORIG_SDL3_CALL(SDL_GetError, ()));
        return -1;
    }

    int ret = copyPixelRows(screenSurf->pixels, screenSurf->pitch);

    if (mustlock)
        ORIG_SDL3_CALL(SDL_UnlockSurface, (screenSurf));

    return ret;
}

int ScreenCapture_SDL3_Surface::copySurfaceToScreen()
{
    GlobalNative gn;

    sdl3::SDL_Surface* surf = ORIG_SDL3_CALL(SDL_GetWindowSurface, (sdl::gameSDLWindow));
    if (!surf || !screenSurf)
        return -1;

    /* Save and restore the clip rectangle */
    sdl3::SDL_Rect clip_rect;
    ORIG_SDL3_CALL(SDL_GetSurfaceClipRect, (surf, &clip_rect));
    ORIG_SDL3_CALL(SDL_SetSurfaceClipRect, (surf, nullptr));
    ORIG_SDL3_CALL(SDL_BlitSurface, (screenSurf, nullptr, surf, nullptr));
    ORIG_SDL3_CALL(SDL_SetSurfaceClipRect, (surf, &clip_rect));

    return 0;
}

void ScreenCapture_SDL3_Surface::restoreScreenState()
{
    copySurfaceToScreen();
}

}
