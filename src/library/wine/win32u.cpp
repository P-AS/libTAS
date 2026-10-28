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

/* Hooks of the unix side of wine `win32u` syscalls, which are exported by
 * `win32u.so` (wine >= 7). Windows-side functions such as GetAsyncKeyState()
 * from user32.dll end up calling these. Unix-side functions use the native
 * calling convention, even on i386. */

#include "win32u.h"
#include "ntdll.h"
#include "winehook.h"

#include "hookpatch.h"
#include "logging.h"
#include "inputs/winekeyboardlayout.h"
#include "inputs/inputs.h"
#include "xlib/XlibGameWindow.h"
#include "xlib/XlibEventQueueList.h"
#include "xlib/xdisplay.h"
#include "xlib/xevents.h"
#include "general/sleepwrappers.h"
#include "GlobalState.h"
#include "checkpoint/ThreadManager.h"
#include "../shared/inputs/SingleInput.h"

#include <X11/keysym.h>
#include <cstdint>
#include <ctime>

namespace libtas {
#if defined(__i386__) || defined(__x86_64__)

typedef struct tagPOINT {
    int32_t x;
    int32_t y;
} POINT;

namespace orig {

static short (*NtUserGetAsyncKeyState)(int key);
static short (*NtUserGetKeyState)(int vkey);
static int (*NtUserGetKeyboardState)(unsigned char *state);
static int (*NtUserGetCursorPos)(POINT *pt);
static uint32_t (*NtUserMsgWaitForMultipleObjectsEx)(uint32_t count, void* const *handles, uint32_t timeout, uint32_t mask, uint32_t flags);
static int (*NtUserGetMessage)(void *msg, void *hwnd, uint32_t first, uint32_t last);
static int (*NtUserPeekMessage)(void *msg, void *hwnd, uint32_t first, uint32_t last, uint32_t flags);

}

static const uint32_t WAIT_TIMEOUT = 0x102;
static const uint32_t INFINITE = 0xFFFFFFFF;
static const uint32_t MWMO_ALERTABLE = 0x2;
static const uint32_t MWMO_INPUTAVAILABLE = 0x4;
static const uint32_t QS_ALLINPUT = 0x4FF;

static bool isKeysymPressed(KeySym ks)
{
    if (ks == NoSymbol)
        return false;

    for (int i=0; i<AllInputs::MAXKEYS; i++) {
        if (Inputs::game_ai.keyboard[i] == 0) {
            break;
        }
        if (Inputs::game_ai.keyboard[i] == ks) {
            return true;
        }
    }
    return false;
}

static bool isVKeyPressed(int vkey)
{
    switch (vkey) {
        /* Mouse buttons. libTAS uses Xlib numbering, where button 2 is the
         * middle button and button 3 is the right button. */
        case 0x01: // VK_LBUTTON
            return Inputs::game_ai.pointer.mask & (1 << SingleInput::POINTER_B1);
        case 0x02: // VK_RBUTTON
            return Inputs::game_ai.pointer.mask & (1 << SingleInput::POINTER_B3);
        case 0x04: // VK_MBUTTON
            return Inputs::game_ai.pointer.mask & (1 << SingleInput::POINTER_B2);
        case 0x05: // VK_XBUTTON1
        case 0x06: // VK_XBUTTON2
            return false;

        /* Generic modifier keys match both left and right keys */
        case 0x10: // VK_SHIFT
            return isKeysymPressed(XK_Shift_L) || isKeysymPressed(XK_Shift_R);
        case 0x11: // VK_CONTROL
            return isKeysymPressed(XK_Control_L) || isKeysymPressed(XK_Control_R);
        case 0x12: // VK_MENU
            return isKeysymPressed(XK_Alt_L) || isKeysymPressed(XK_Alt_R);

        default:
            if ((vkey < 0) || (vkey > 0xff))
                return false;
            return isKeysymPressed(VKeyToXKeysym(vkey));
    }
}

short NtUserGetAsyncKeyState(int key)
{
    LOGTRACE(LCF_WINE | LCF_KEYBOARD, "%s called with key %d", __func__, key);
    return isVKeyPressed(key) ? static_cast<short>(0x8000) : 0;
}

short NtUserGetKeyState(int vkey)
{
    LOGTRACE(LCF_WINE | LCF_KEYBOARD, "%s called with key %d", __func__, vkey);
    /* Low-order bit is the toggle state, which we don't track */
    return isVKeyPressed(vkey) ? static_cast<short>(0x8000) : 0;
}

int NtUserGetKeyboardState(unsigned char *state)
{
    LOGTRACE_SIMPLE(LCF_WINE | LCF_KEYBOARD);
    if (!state)
        return 0;

    for (int vkey=0; vkey<256; vkey++) {
        state[vkey] = isVKeyPressed(vkey) ? 0x80 : 0;
    }
    return 1;
}

int NtUserGetCursorPos(POINT *pt)
{
    LOGTRACE_SIMPLE(LCF_WINE | LCF_MOUSE);
    if (!pt)
        return 0;

    /* libTAS pointer coordinates are relative to the game window, while wine
     * uses screen coordinates. Use the window position that we report to
     * wine, so that ScreenToClient() gives back our pointer coordinates. */
    int x, y;
    XlibGameWindow::getCoords(XlibGameWindow::get(), &x, &y);
    pt->x = Inputs::game_ai.pointer.x + x;
    pt->y = Inputs::game_ai.pointer.y + y;
    LOG(LL_DEBUG, LCF_WINE | LCF_MOUSE, "   returning (%d,%d)", pt->x, pt->y);
    return 1;
}

/* winex11 only reads X events when wineserver notices that the X connection
 * of the thread is readable. The events that we insert in our Xlib event
 * queue (inputs, and the native events that we pulled) don't make it
 * readable, so they stayed there until an unrelated X event arrived, for
 * example when the game window had the focus. Before the game looks at its
 * messages, make the connection readable and wait for winex11 to process
 * our queue, so that the game gets the events of the current frame. */
static void processXlibEvents()
{
    Display *display = x11::threadDisplay;
    if (!display)
        return;

    std::shared_ptr<XlibEventQueue> queue = xlibEventQueueList.getQueue(display);
    if (!queue || queue->size() == 0)
        return;

    LOGTRACE(LCF_WINE | LCF_EVENTS, "%s with %d events", __func__, queue->size());

    sendXlibWakeEvent(display);

    /* Wine processes driver events inside the wait once wineserver has seen
     * the connection readable. The wake event is pulled by the same
     * X11DRV_ProcessEvents() call that empties our queue (or that leaves it
     * alone, when called while handling an X event). The wait itself never
     * returns early, because the processing clears the queue bit that would
     * signal it. */
    for (int i = 0; isXlibWakeEventPending(display); i++) {
        if (i == 100) {
            LOG(LL_DEBUG, LCF_WINE | LCF_EVENTS, "   timeout while waiting for wine to process X events");
            break;
        }
        orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 1, 0, 0);
    }
}

static uint32_t NtUserMsgWaitForMultipleObjectsEx(uint32_t count, void* const *handles, uint32_t timeout, uint32_t mask, uint32_t flags)
{
    if (GlobalState::isNative())
        return orig::NtUserMsgWaitForMultipleObjectsEx(count, handles, timeout, mask, flags);

    processXlibEvents();

    /* Infinite and alertable waits are left to wine */
    if ((timeout == 0) || (timeout == INFINITE) || (flags & MWMO_ALERTABLE))
        return orig::NtUserMsgWaitForMultipleObjectsEx(count, handles, timeout, mask, flags);

    LOGTRACE(LCF_WINE | LCF_WAIT, "%s call with timeout %u ms", __func__, timeout);

    /* Wine waits on the real time, while the timeout was computed by the game
     * from our deterministic time, so the game would be stuck waiting
     * without advancing time. Check without waiting, and transfer the
     * timeout as a sleep if nothing was signaled. */
    uint32_t ret = orig::NtUserMsgWaitForMultipleObjectsEx(count, handles, 0, mask, flags);
    if (ret != WAIT_TIMEOUT)
        return ret;

    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    transfer_sleep(ts, nullptr);
    return WAIT_TIMEOUT;
}

static int NtUserPeekMessage(void *msg, void *hwnd, uint32_t first, uint32_t last, uint32_t flags)
{
    if (!GlobalState::isNative())
        processXlibEvents();

    return orig::NtUserPeekMessage(msg, hwnd, first, last, flags);
}

static int NtUserGetMessage(void *msg, void *hwnd, uint32_t first, uint32_t last)
{
    if (!GlobalState::isNative())
        processXlibEvents();

    if (GlobalState::isNative() || !ThreadManager::isMainThread() || hwnd || first || last)
        return orig::NtUserGetMessage(msg, hwnd, first, last);

    LOGTRACE_SIMPLE(LCF_WINE | LCF_EVENTS);

    /* The game may wait for a message posted from a timer thread, which
     * waits until our deterministic time reaches its deadline. Advance time
     * while no message is available, otherwise both threads wait forever. */
    struct timespec mssleep = {0, 1000000};
    while (orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 0, QS_ALLINPUT, MWMO_INPUTAVAILABLE) == WAIT_TIMEOUT) {
        transfer_sleep(mssleep, nullptr);

        /* Give other threads the chance to post their message */
        orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 1, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    }

    return orig::NtUserGetMessage(msg, hwnd, first, last);
}

void hook_win32u()
{
    /* Patching twice would make the trampoline call our own function */
    static bool hooked = false;
    if (hooked)
        return;
    hooked = true;

    /* win32u is loaded after wine mapped the shared user data page, and
     * before the game code runs */
    takeover_wine_shared_data();

    HOOK_PATCH_ORIG(NtUserGetAsyncKeyState, "win32u.so");
    HOOK_PATCH_ORIG(NtUserGetKeyState, "win32u.so");
    HOOK_PATCH_ORIG(NtUserGetKeyboardState, "win32u.so");
    HOOK_PATCH_ORIG(NtUserGetCursorPos, "win32u.so");

    /* Only redirect the waits coming from the Windows side */
    hook_wine_syscall("win32u.so", "NtUserMsgWaitForMultipleObjectsEx",
        reinterpret_cast<void**>(&orig::NtUserMsgWaitForMultipleObjectsEx),
        reinterpret_cast<void*>(NtUserMsgWaitForMultipleObjectsEx));
    hook_wine_syscall("win32u.so", "NtUserGetMessage",
        reinterpret_cast<void**>(&orig::NtUserGetMessage),
        reinterpret_cast<void*>(NtUserGetMessage));
    hook_wine_syscall("win32u.so", "NtUserPeekMessage",
        reinterpret_cast<void**>(&orig::NtUserPeekMessage),
        reinterpret_cast<void*>(NtUserPeekMessage));
}

#else
void hook_win32u()
{
}
#endif
}
