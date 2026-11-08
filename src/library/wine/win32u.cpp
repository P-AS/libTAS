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
#include "DeterministicTimer.h"
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
#include "global.h"
#include "UnityHacks.h"
#include "checkpoint/ThreadManager.h"
#include "../shared/inputs/SingleInput.h"

#include <X11/keysym.h>
#include <atomic>
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
static uint32_t (*NtUserSendInput)(uint32_t count, void *inputs, int size);
static void (*client_surface_present)(void *surface);
static void (*client_surface_add_ref)(void *surface);
static void (*client_surface_release)(void *surface);

}

static const uint32_t WAIT_TIMEOUT = 0x102;
static const uint32_t INFINITE = 0xFFFFFFFF;
static const uint32_t MWMO_ALERTABLE = 0x2;
static const uint32_t MWMO_INPUTAVAILABLE = 0x4;
static const uint32_t QS_ALLINPUT = 0x4FF;

/* Keys that the game itself pressed with SendInput(), such as the keyboard
 * output of Xidi, which turns the buttons of a controller into key presses.
 * They are not libTAS inputs, but a game that reads the keyboard state must
 * see them. */
static std::atomic<bool> injected_vkeys[256];

/* Virtual key of a scan code (set 1, US layout) */
static int scanToVKey(unsigned int scan, bool extended)
{
    static const unsigned char normal[0x59] = {
        0, 0x1B, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0xBD, 0xBB, 0x08, 0x09, // 0x00
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0xDB, 0xDD, 0x0D, 0xA2, 'A', 'S', // 0x10
        'D', 'F', 'G', 'H', 'J', 'K', 'L', 0xBA, 0xDE, 0xC0, 0xA0, 0xDC, 'Z', 'X', 'C', 'V', // 0x20
        'B', 'N', 'M', 0xBC, 0xBE, 0xBF, 0xA1, 0x6A, 0xA4, 0x20, 0x14, 0x70, 0x71, 0x72, 0x73, 0x74, // 0x30
        0x75, 0x76, 0x77, 0x78, 0x79, 0x90, 0x91, 0x67, 0x68, 0x69, 0x6D, 0x64, 0x65, 0x66, 0x6B, 0x61, // 0x40
        0x62, 0x63, 0x60, 0x6E, 0, 0, 0, 0x7A, 0x7B // 0x50
    };
    if (extended) {
        switch (scan) {
            case 0x1C: return 0x0D; // numpad enter
            case 0x1D: return 0xA3; // right control
            case 0x35: return 0x6F; // divide
            case 0x38: return 0xA5; // right alt
            case 0x47: return 0x24; // home
            case 0x48: return 0x26; // up
            case 0x49: return 0x21; // prior
            case 0x4B: return 0x25; // left
            case 0x4D: return 0x27; // right
            case 0x4F: return 0x23; // end
            case 0x50: return 0x28; // down
            case 0x51: return 0x22; // next
            case 0x52: return 0x2D; // insert
            case 0x53: return 0x2E; // delete
            case 0x5B: return 0x5B; // left windows
            case 0x5C: return 0x5C; // right windows
            default: return 0;
        }
    }
    if (scan < sizeof(normal))
        return normal[scan];
    return 0;
}

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
    /* Keys pressed by the game with SendInput(), the generic modifiers match
     * the left and right keys */
    if ((vkey >= 0) && (vkey <= 0xff)) {
        if (injected_vkeys[vkey])
            return true;
        if ((vkey == 0x10) && (injected_vkeys[0xA0] || injected_vkeys[0xA1]))
            return true;
        if ((vkey == 0x11) && (injected_vkeys[0xA2] || injected_vkeys[0xA3]))
            return true;
        if ((vkey == 0x12) && (injected_vkeys[0xA4] || injected_vkeys[0xA5]))
            return true;
    }

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


/* INPUT of the 64-bit syscall (WoW64 converts the one of 32-bit programs) */
struct WineKeybdInput {
    uint32_t type; // INPUT_KEYBOARD is 1
    uint32_t pad;
    uint16_t vk;
    uint16_t scan;
    uint32_t flags; // 1: extended, 2: key up, 4: unicode, 8: scan code
    uint32_t time;
    uint32_t pad2;
    uint64_t extra;
    uint64_t pad3;
};

uint32_t NtUserSendInput(uint32_t count, void *inputs, int size)
{
    LOGTRACE(LCF_WINE | LCF_KEYBOARD, "%s called with %u inputs of size %d", __func__, count, size);
    if (inputs && (size == static_cast<int>(sizeof(WineKeybdInput)))) {
        const WineKeybdInput* in = static_cast<const WineKeybdInput*>(inputs);
        for (uint32_t i = 0; i < count; i++) {
            if (in[i].type != 1)
                continue;

            const uint32_t flags = in[i].flags;
            if (flags & 0x4) // KEYEVENTF_UNICODE
                continue;

            int vkey = in[i].vk;
            if (flags & 0x8) { // KEYEVENTF_SCANCODE
                const bool extended = (flags & 0x1) || ((in[i].scan >> 8) == 0xE0);
                vkey = scanToVKey(in[i].scan & 0xFF, extended);
            }
            const bool down = !(flags & 0x2); // KEYEVENTF_KEYUP
            LOG(LL_DEBUG, LCF_WINE | LCF_KEYBOARD, "   SendInput key vk 0x%x scan 0x%x flags 0x%x -> vkey 0x%x %s", in[i].vk, in[i].scan, flags, vkey, down ? "down" : "up");
            if ((vkey > 0) && (vkey <= 0xff))
                injected_vkeys[vkey] = down;
        }
    }
    return orig::NtUserSendInput(count, inputs, size);
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

    /* The frame boundary already sleeps until the end of the frame. As the
     * game time doesn't advance while the game works, a timeout up to the
     * next frame slept in real time would add to the time of that work, and
     * the game would run below its framerate. */
    if (ThreadManager::isMainThread() && !UnityHacks::isUnity() &&
        (Global::shared_config.sleep_handling != SharedConfig::SLEEP_NEVER))
        DeterministicTimer::get().addDelay(ts);
    else
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
     * while no message is available, otherwise both threads wait forever.
     * Only advance it when the other threads are idle: the first thread of
     * Rayman 2 only handles messages while its engine thread starts, and
     * advancing time at the pace of the real time it takes to start made the
     * number of non-draw frames at startup vary between runs. Stop doing it
     * when another thread runs the frame boundaries: this thread then only
     * waits for its messages. */
    struct timespec mssleep = {0, 1000000};
    while (orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 0, QS_ALLINPUT, MWMO_INPUTAVAILABLE) == WAIT_TIMEOUT) {
        if (!ThreadManager::isMainThread() || !ThreadManager::isFrameThread())
            break;
        DeterministicTimer::get().waitForIdleThreads(2000000000LL);
        if (orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 0, QS_ALLINPUT, MWMO_INPUTAVAILABLE) != WAIT_TIMEOUT)
            break;
        transfer_sleep(mssleep, nullptr);

        /* Give other threads the chance to post their message */
        orig::NtUserMsgWaitForMultipleObjectsEx(0, nullptr, 1, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
    }

    return orig::NtUserGetMessage(msg, hwnd, first, last);
}

/* Last client surface presented by wine */
static void *last_client_surface = nullptr;

void client_surface_present(void *surface)
{
    if (!GlobalState::isNative() && surface && (surface != last_client_surface) &&
        orig::client_surface_add_ref && orig::client_surface_release) {
        /* Keep a reference, so that the surface stays valid */
        orig::client_surface_add_ref(surface);
        if (last_client_surface)
            orig::client_surface_release(last_client_surface);
        last_client_surface = surface;
    }

    orig::client_surface_present(surface);
}

void present_wine_client_surface()
{
    if (last_client_surface && orig::client_surface_present)
        orig::client_surface_present(last_client_surface);
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

    /* Wine presents offscreen OpenGL/Vulkan surfaces (child windows) into the
     * game window itself after the swap, so we must present again after
     * the swaps that we perform during the frame boundary */
    HOOK_PATCH_ORIG(client_surface_present, "win32u.so");
    orig::client_surface_add_ref = reinterpret_cast<decltype(orig::client_surface_add_ref)>(find_wine_symbol("win32u.so", "client_surface_add_ref"));
    orig::client_surface_release = reinterpret_cast<decltype(orig::client_surface_release)>(find_wine_symbol("win32u.so", "client_surface_release"));

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
    hook_wine_syscall("win32u.so", "NtUserSendInput",
        reinterpret_cast<void**>(&orig::NtUserSendInput),
        reinterpret_cast<void*>(NtUserSendInput));
}

#else
void hook_win32u()
{
}

void present_wine_client_surface()
{
}
#endif
}
