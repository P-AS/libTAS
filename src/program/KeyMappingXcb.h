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

#ifndef LIBTAS_KEYMAPPING_XCB_H_INCLUDED
#define LIBTAS_KEYMAPPING_XCB_H_INCLUDED

#include "KeyMapping.h"
#include "pointer/GlobalPointer.h"
#include "pointer/PointerCalibration.h"

#include <memory>
#include <string>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

struct SharedConfig;
class AllInputs;

class KeyMappingXcb : public KeyMapping {
    public:
        /* Initialize hotkeys and mapping list */
        KeyMappingXcb(void* conn);

        /* Returns if a keysym is a modifier */
        bool is_modifier(keysym_t ks);

        /* Returns the list of modifiers compated into a single int from the event state */
        keysym_t get_modifiers(int state);

        /* Get the input description with modifiers */
        std::string input_description_mod(keysym_t ks);

        /* Convert a native keycode into xcb keysym */
        keysym_t nativeToKeysym(int keycode);

        /* Builds the keysym to keysym without modifier map */
        void base_keysyms();

        /* Set hotkeys to default values */
        void default_inputs();

        /* Set input to default value */
        void default_input(int tab, int input_index);

        /*
         * We are building the whole AllInputs structure,
         * that will be passed to the game and saved.
         * We will be doing the following steps:
         * - Get the raw keyboard state using XQueryKeymap
         * - Convert keyboard keycodes (physical keys) to keysyms (key meaning)
         * - Check if the keysym is mapped to a hotkey. If so, we skip it
         * - Check if the key is mapped to another input and fill the AllInputs struct accordingly
         * - Get the mouse state
         * - Warp mouse pointer if needed
         */
        void buildAllInputs(AllInputs& ai, uint32_t window, SharedConfig& sc, bool mouse_warp);

        /*
         * Xwayland only honors a pointer warp while the cursor is hidden, by
         * locking the Wayland pointer and feeding relative motion to the X
         * pointer. Otherwise, the warp only lasts until the next real motion.
         * So on Xwayland, we hold the pointer with a grab that hides the cursor
         * and confines it to the window: without the confinement, a motion
         * going past the window edge would break the lock for good.
         * Does nothing on a real X server, where warping just works.
         */
        void capturePointer(uint32_t window, bool capture);

        void setPointerOutsideSource(int source);

        bool readsPointerOutside() const;

    private:
        /* Connection to the X11 server */
        xcb_connection_t *conn;

        /* Connection to the keyboard layout */
        xcb_key_symbols_t *keysyms;

        /* Map keyboard KeySym with modifiers to keyboard KeySym without modifiers */
        std::map<keysym_t,keysym_t> keysym_mapping;

        /* Returns the list of modifiers from the keyboard state */
        keysym_t build_modifiers(unsigned char keyboard_state[]);

        /* Are we running on Xwayland */
        bool is_xwayland = false;

        /* Window that the pointer is held in, if any */
        xcb_window_t captured_window = XCB_NONE;

        /* Invisible cursor shown during our pointer grabs */
        xcb_cursor_t blank_cursor = XCB_NONE;

        /* Grab the pointer, hiding the cursor and confining it to the window */
        bool grabPointer(xcb_window_t window);

        /* Warp the pointer at the center of a window of the given size, in a
         * way that really moves the pointer. */
        void warpPointer(xcb_window_t window, int width, int height);

        /* Where to read the pointer position when Xwayland can't tell */
        int pointer_outside_source = GlobalPointer::SOURCE_NONE;
        std::unique_ptr<GlobalPointer> global_pointer;
        bool global_pointer_started = false;
        PointerCalibration pointer_calibration;
        xcb_window_t calibrated_window = XCB_NONE;

        /* Top-level window of the window that was last asked */
        xcb_window_t toplevel_of = XCB_NONE;
        xcb_window_t toplevel = XCB_NONE;

        /* Is this direct child of the root the one holding the window */
        bool isToplevelOf(xcb_window_t root_child, xcb_window_t window);
};

#endif // LIBTAS_KEYMAPPING_XCB_H_INCLUDED
