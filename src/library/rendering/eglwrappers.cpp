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

#include "eglwrappers.h"
#include "openglwrappers.h"
#include "openglloader.h"

#include "hook.h"
#include "global.h"
#include "logging.h"
#include "renderhud/RenderHUD_GL.h"
#include "screencapture/ScreenCapture.h"
#include "frame.h"
#include "xlib/XlibGameWindow.h"
#include "GlobalState.h"
#ifdef __linux__
#include "wine/winehook.h"
#include "wine/win32u.h"
#endif

#include <string.h>
#include <map>
#include <X11/Xlib.h>

#define RETURN_SYMBOL(str) \
    if (!strcmp(symbol, #str)) { \
        LOGTRACE(LCF_OGL,"  return my symbol %p, real function in %p", reinterpret_cast<void*>(libtas::str), real_pointer); \
        return reinterpret_cast<void*>(libtas::str); \
    }

#define RETURN_SYMBOL_CUSTOM(str) \
    if (!strcmp(symbol, #str)) { \
        LOGTRACE(LCF_OGL,"  return my symbol %p, real function in %p", reinterpret_cast<void*>(my##str), real_pointer); \
        return reinterpret_cast<void*>(my##str); \
    }

namespace libtas {

DEFINE_ORIG_POINTER(eglGetProcAddress)
DEFINE_ORIG_POINTER(eglMakeCurrent)
DEFINE_ORIG_POINTER(eglSwapBuffers)
DEFINE_ORIG_POINTER(eglSwapInterval)
DEFINE_ORIG_POINTER(eglBindAPI)
DEFINE_ORIG_POINTER(eglCreateContext)
DEFINE_ORIG_POINTER(eglCreateWindowSurface)
DEFINE_ORIG_POINTER(eglCreatePlatformWindowSurface)
DEFINE_ORIG_POINTER(eglCreatePlatformWindowSurfaceEXT)

/* If the game uses the eglGetProcAddress functions to access to a function
 * that we hook, we must return our function and store the original pointers
 * so that we can call the real function.
 */
static void* store_orig_and_return_my_symbol(const char* symbol, void* real_pointer) {

    if (!real_pointer || !symbol)
        return real_pointer;

    RETURN_SYMBOL(eglMakeCurrent)
    RETURN_SYMBOL(eglSwapBuffers)
    RETURN_SYMBOL(eglSwapInterval)
    RETURN_SYMBOL(eglBindAPI)
    RETURN_SYMBOL(eglCreateContext)

    /* Extension functions may only be available from here */
    if (!strcmp(symbol, "eglCreatePlatformWindowSurfaceEXT")) {
        orig::eglCreatePlatformWindowSurfaceEXT = reinterpret_cast<decltype(orig::eglCreatePlatformWindowSurfaceEXT)>(real_pointer);
        return reinterpret_cast<void*>(libtas::eglCreatePlatformWindowSurfaceEXT);
    }
    RETURN_SYMBOL(eglCreateWindowSurface)
    RETURN_SYMBOL(eglCreatePlatformWindowSurface)

    /* Store function pointers that are used in other files */
    RETURN_SYMBOL_CUSTOM(glBlitFramebuffer);
    RETURN_SYMBOL_CUSTOM(glTexParameterf);
    RETURN_SYMBOL_CUSTOM(glTexParameteri);
    RETURN_SYMBOL_CUSTOM(glEnable);
    RETURN_SYMBOL_CUSTOM(glDrawElements)
    
    RETURN_SYMBOL_CUSTOM(glClear)

    RETURN_SYMBOL_CUSTOM(glBegin);
    RETURN_SYMBOL_CUSTOM(glEnd);

    RETURN_SYMBOL_CUSTOM(glVertex2d);
    RETURN_SYMBOL_CUSTOM(glVertex2f);
    RETURN_SYMBOL_CUSTOM(glVertex2i);
    RETURN_SYMBOL_CUSTOM(glVertex2s);

    RETURN_SYMBOL_CUSTOM(glVertex3d);
    RETURN_SYMBOL_CUSTOM(glVertex3f);
    RETURN_SYMBOL_CUSTOM(glVertex3i);
    RETURN_SYMBOL_CUSTOM(glVertex3s);

    RETURN_SYMBOL_CUSTOM(glVertex4d);
    RETURN_SYMBOL_CUSTOM(glVertex4f);
    RETURN_SYMBOL_CUSTOM(glVertex4i);
    RETURN_SYMBOL_CUSTOM(glVertex4s);

    RETURN_SYMBOL_CUSTOM(glVertex2dv);
    RETURN_SYMBOL_CUSTOM(glVertex2fv);
    RETURN_SYMBOL_CUSTOM(glVertex2iv);
    RETURN_SYMBOL_CUSTOM(glVertex2sv);

    RETURN_SYMBOL_CUSTOM(glVertex3dv);
    RETURN_SYMBOL_CUSTOM(glVertex3fv);
    RETURN_SYMBOL_CUSTOM(glVertex3iv);
    RETURN_SYMBOL_CUSTOM(glVertex3sv);

    RETURN_SYMBOL_CUSTOM(glVertex4dv);
    RETURN_SYMBOL_CUSTOM(glVertex4fv);
    RETURN_SYMBOL_CUSTOM(glVertex4iv);
    RETURN_SYMBOL_CUSTOM(glVertex4sv);

    RETURN_SYMBOL_CUSTOM(glDrawArrays)
    RETURN_SYMBOL_CUSTOM(glMultiDrawArrays)
    RETURN_SYMBOL_CUSTOM(glMultiDrawElements)

    RETURN_SYMBOL_CUSTOM(glDrawRangeElements);
    RETURN_SYMBOL_CUSTOM(glDrawElementsBaseVertex);
    RETURN_SYMBOL_CUSTOM(glDrawRangeElementsBaseVertex);
    RETURN_SYMBOL_CUSTOM(glDrawElementsInstancedBaseVertex);
    RETURN_SYMBOL_CUSTOM(glMultiDrawElementsBaseVertex);
    RETURN_SYMBOL_CUSTOM(glDrawArraysInstancedBaseInstance);
    RETURN_SYMBOL_CUSTOM(glDrawElementsInstancedBaseInstance);
    RETURN_SYMBOL_CUSTOM(glDrawElementsInstancedBaseVertexBaseInstance);

    RETURN_SYMBOL_CUSTOM(glDrawTransformFeedback);
    RETURN_SYMBOL_CUSTOM(glDrawTransformFeedbackStream);
    RETURN_SYMBOL_CUSTOM(glDrawTransformFeedbackInstanced);
    RETURN_SYMBOL_CUSTOM(glDrawTransformFeedbackStreamInstanced);

    RETURN_SYMBOL_CUSTOM(glDrawArraysInstancedARB);
    RETURN_SYMBOL_CUSTOM(glDrawElementsInstancedARB);
    RETURN_SYMBOL_CUSTOM(glDrawArraysInstancedEXT);
    RETURN_SYMBOL_CUSTOM(glDrawElementsInstancedEXT);
    RETURN_SYMBOL_CUSTOM(glDrawRangeElementsEXT);
    RETURN_SYMBOL_CUSTOM(glMultiDrawArraysEXT);
    RETURN_SYMBOL_CUSTOM(glMultiDrawElementsEXT);
    RETURN_SYMBOL_CUSTOM(glDrawArraysEXT);

    return real_pointer;
}

void(*eglGetProcAddress (const char *procName))()
{
    LOGTRACE(LCF_OGL, "%s call with symbol %s", __func__, procName);
    LINK_NAMESPACE(eglGetProcAddress, "EGL");

    if (!orig::eglGetProcAddress) return nullptr;

    gl_load_procs(reinterpret_cast<GLGetProcAddressProc>(orig::eglGetProcAddress));

    return reinterpret_cast<void(*)()>(store_orig_and_return_my_symbol(procName, reinterpret_cast<void*>(orig::eglGetProcAddress(procName))));
}

EGLBoolean eglMakeCurrent( EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx )
{
    LINK_NAMESPACE(eglMakeCurrent, "EGL");

    EGLBoolean ret = orig::eglMakeCurrent(dpy, draw, read, ctx);
    if (GlobalState::isNative())
        return ret;

    LOGTRACE_SIMPLE(LCF_WINDOW | LCF_OGL);

    if (draw && XlibGameWindow::get()) {
        Global::game_info.video |= GameInfo::EGL | GameInfo::OPENGL;
        Global::game_info.tosend = true;
    }

    return ret;
}

static EGLenum bindAPI = EGL_OPENGL_ES_API;

EGLBoolean eglBindAPI(EGLenum api)
{
    LOGTRACE(LCF_OGL, "%s call with API %d", __func__, api);
    LINK_NAMESPACE(eglBindAPI, "EGL");

    bindAPI = api;

    return orig::eglBindAPI(api);
}

/* X11 window of each window surface, and its top-level window */
static std::map<EGLSurface, std::pair<Window, Window>> surfaceWindows;

static void registerSurfaceWindow(EGLSurface surface, Window w)
{
    /* For other games, the native window may not be a X11 window, and a
     * request on an invalid window would make Xlib exit the game. Wine sets
     * its own X error handler. */
#ifdef __linux__
    if (!isWineGame())
        return;
#else
    return;
#endif

    if ((surface == EGL_NO_SURFACE) || !w)
        return;

    surfaceWindows[surface] = std::make_pair(w, static_cast<Window>(0));
}

/* Returns the top-level window of the X11 window that the surface draws to */
static Window getSurfaceTopLevelWindow(EGLSurface surface)
{
    auto it = surfaceWindows.find(surface);
    if (it == surfaceWindows.end())
        return 0;

    if (!it->second.second)
        it->second.second = XlibGameWindow::topLevel(it->second.first);
    return it->second.second;
}

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    LINK_NAMESPACE(eglCreateWindowSurface, "EGL");

    EGLSurface surface = orig::eglCreateWindowSurface(dpy, config, win, attrib_list);
    if (GlobalState::isNative())
        return surface;

    LOGTRACE(LCF_WINDOW | LCF_OGL, "%s call with window %d", __func__, static_cast<Window>(win));
    registerSurfaceWindow(surface, static_cast<Window>(win));
    return surface;
}

EGLSurface eglCreatePlatformWindowSurface(EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list)
{
    LINK_NAMESPACE(eglCreatePlatformWindowSurface, "EGL");

    EGLSurface surface = orig::eglCreatePlatformWindowSurface(dpy, config, native_window, attrib_list);
    if (GlobalState::isNative() || !native_window)
        return surface;

    /* For the X11 platform, the native window is a pointer to the window */
    Window w = *static_cast<Window*>(native_window);
    LOGTRACE(LCF_WINDOW | LCF_OGL, "%s call with window %d", __func__, w);
    registerSurfaceWindow(surface, w);
    return surface;
}

EGLSurface eglCreatePlatformWindowSurfaceEXT(EGLDisplay dpy, EGLConfig config, void *native_window, const EGLint *attrib_list)
{
    if (!orig::eglCreatePlatformWindowSurfaceEXT) {
        LINK_NAMESPACE(eglGetProcAddress, "EGL");
        if (orig::eglGetProcAddress)
            orig::eglCreatePlatformWindowSurfaceEXT = reinterpret_cast<decltype(orig::eglCreatePlatformWindowSurfaceEXT)>(orig::eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT"));
        if (!orig::eglCreatePlatformWindowSurfaceEXT)
            return EGL_NO_SURFACE;
    }

    EGLSurface surface = orig::eglCreatePlatformWindowSurfaceEXT(dpy, config, native_window, attrib_list);
    if (GlobalState::isNative() || !native_window)
        return surface;

    Window w = *static_cast<Window*>(native_window);
    LOGTRACE(LCF_WINDOW | LCF_OGL, "%s call with window %d", __func__, w);
    registerSurfaceWindow(surface, w);
    return surface;
}

EGLBoolean eglSwapBuffers( EGLDisplay dpy, EGLSurface surface )
{
    LINK_NAMESPACE(eglSwapBuffers, "EGL");

    if (GlobalState::isNative())
        return orig::eglSwapBuffers(dpy, surface);

    LOGTRACE_SIMPLE(LCF_WINDOW | LCF_OGL);

    /* Games may map other windows after the one they draw to, so make sure
     * that the window we draw to is the game window */
    XlibGameWindow::setDrawn(getSurfaceTopLevelWindow(surface));

    /* Start the frame boundary and pass the function to draw */
    static RenderHUD_GL renderHUD;
    renderHUD.setGLES(bindAPI == EGL_OPENGL_ES_API);
    frameBoundary([&] () {
        NATIVECALL(orig::eglSwapBuffers(dpy, surface));
        /* Wine copies offscreen surfaces into the game window only after
         * this swap returns, so do it now for frames shown while paused */
        if (isWineGame())
            present_wine_client_surface();
    }, renderHUD);

    return EGL_TRUE;
}

static int swapInterval = 0;

EGLBoolean eglSwapInterval (EGLDisplay dpy, EGLint interval)
{
    LOGTRACE(LCF_OGL, "%s call with interval %d", __func__, interval);
    LINK_NAMESPACE(eglSwapInterval, "EGL");

    swapInterval = interval;

    /* When using non deterministic timer, we let the game set vsync */
    if (Global::shared_config.debug_state & SharedConfig::DEBUG_UNCONTROLLED_TIME) {
        return orig::eglSwapInterval(dpy, interval);
    }
    return orig::eglSwapInterval(dpy, interval);
}

EGLContext eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    LOGTRACE_SIMPLE(LCF_OGL);
    LINK_NAMESPACE(eglCreateContext, "EGL");

    if (!attrib_list) {
        if (bindAPI == EGL_OPENGL_ES_API)
            Global::game_info.opengl_profile = GameInfo::ES;

        Global::game_info.tosend = true;
        return orig::eglCreateContext(dpy, config, share_context, attrib_list);
    }

    int i = 0;
    while (attrib_list[i] != 0) {
        if (attrib_list[i] == EGL_CONTEXT_MAJOR_VERSION) {
            Global::game_info.opengl_major = attrib_list[i+1];
        }
        if (attrib_list[i] == EGL_CONTEXT_MINOR_VERSION) {
            Global::game_info.opengl_minor = attrib_list[i+1];
        }
        if (attrib_list[i] == EGL_CONTEXT_OPENGL_PROFILE_MASK) {
            switch(attrib_list[i+1]) {
            case EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT:
                Global::game_info.opengl_profile = GameInfo::CORE;
                break;
            case EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT:
                Global::game_info.opengl_profile = GameInfo::COMPATIBILITY;
                break;
            default:
                break;
            }
        }

        i += 2;
    }
    
    if (bindAPI == EGL_OPENGL_ES_API)
        Global::game_info.opengl_profile = GameInfo::ES;
    
    Global::game_info.tosend = true;
    
    return orig::eglCreateContext (dpy, config, share_context, attrib_list);
}

}
