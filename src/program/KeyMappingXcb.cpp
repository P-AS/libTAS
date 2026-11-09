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

#include "KeyMappingXcb.h"
#include "../shared/inputs/SingleInput.h"
#include "../shared/inputs/AllInputs.h"
#include "../shared/SharedConfig.h"
#include "../external/keysymdesc.h"
#include "../external/keysymdef.h"

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <array>

KeyMappingXcb::KeyMappingXcb(void* c) : KeyMapping(c)
{
    conn = static_cast<xcb_connection_t*>(c);

    static const char xwayland_name[] = "XWAYLAND";
    xcb_query_extension_cookie_t ext_cookie = xcb_query_extension(conn, strlen(xwayland_name), xwayland_name);
    xcb_query_extension_reply_t* ext_reply = xcb_query_extension_reply(conn, ext_cookie, nullptr);
    is_xwayland = ext_reply && ext_reply->present;
    free(ext_reply);

    /* Build the list of keysym values to be mapped based on valid keycodes.
     * This list is dependent on the keyboard layout.
     */
    if (!(keysyms = xcb_key_symbols_alloc(conn))) {
        // std::cerr << "Could not allocate key symbols" << std::endl;
        return;
    }

    /* Add all keysym from LATIN1 (0x00ab) and MISC (0xffab), and check if
     * there is a keycode mapped to it.
     */

     /* LATIN1 */
    for (int ks = 0; ks < 256; ks++) {
        SingleInput si;
        si.type = SingleInput::IT_KEYBOARD;
        si.which = 0x0000 | ks;
        const char* str = KEYSYM_TO_DESC_LATIN(ks);
        if (str) {
            si.description = str;
            input_list[INPUTLIST_KEYBOARD_LATIN].push_back(si);
        }
    }

    /* MISC */
    for (int ks = 0; ks < 256; ks++) {
        SingleInput si;
        si.type = SingleInput::IT_KEYBOARD;
        si.which = 0xff00 | ks;
        const char* str = KEYSYM_TO_DESC_MISC(ks);
        if (str) {
            si.description = str;
            input_list[INPUTLIST_KEYBOARD_MISC].push_back(si);
        }
    }

    /* Build the map between keysym values with modifiers and keysym values without modifiers */
    base_keysyms();

    /* Set default hotkeys */
    default_hotkeys();

    /* Set default inputs */
    default_inputs();
}

struct ModifierKey {
    keysym_t ks;
    keysym_t flag;
    std::string description;
};

static std::array<ModifierKey, 8> modifier_list {{
    {XK_Shift_L, XK_Shift_Flag, "Shift"},
    {XK_Shift_R, XK_Shift_Flag, "Shift"},
    {XK_Control_L, XK_Control_Flag, "Control"},
    {XK_Control_R, XK_Control_Flag, "Control"},
    {XK_Meta_L, XK_Meta_Flag, "Meta"},
    {XK_Meta_R, XK_Meta_Flag, "Meta"},
    {XK_Alt_L, XK_Alt_Flag, "Alt"},
    {XK_Alt_R, XK_Alt_Flag, "Alt"},
}};

bool KeyMappingXcb::is_modifier(keysym_t ks)
{
    for (ModifierKey modifier : modifier_list) {
        if (modifier.ks == ks)
            return true;
    }
    return false;
}

keysym_t KeyMappingXcb::get_modifiers(int state)
{
    keysym_t modifiers = 0;
    if (state & XCB_MOD_MASK_SHIFT)
        modifiers |= XK_Shift_Flag;
    if (state & XCB_MOD_MASK_CONTROL)
        modifiers |= XK_Control_Flag;
    if (state & XCB_MOD_MASK_1)
        modifiers |= XK_Alt_Flag;
    if (state & XCB_MOD_MASK_3)
        modifiers |= XK_Meta_Flag;
    
    return modifiers;
}

keysym_t KeyMappingXcb::build_modifiers(unsigned char keyboard_state[])
{
    xcb_keysym_t modifiers = 0;

    for (int i=0; i<256; i++) {
        if (keyboard_state[i/8] & (1 << (i % 8))) {
            xcb_keysym_t ks = xcb_key_symbols_get_keysym(keysyms, i, 0);
            for (ModifierKey modifier : modifier_list) {
                if (modifier.ks == ks) {
                    modifiers |= modifier.flag;
                    break;
                }
            }
        }
    }
    return modifiers;
}

std::string KeyMappingXcb::input_description_mod(keysym_t ks)
{
    std::string str = "";
    
    for (ModifierKey modifier : modifier_list) {
        if (ks & modifier.flag) {
            str += modifier.description;
            str += "+";
            ks ^= modifier.flag;
        }
    }

    str += input_description(ks);
    return str;
}

void KeyMappingXcb::base_keysyms()
{
    keysym_mapping.clear();

    /* Map all keycode + modifier combinations to the keysym obtained without modifiers */

    /* Gather the list of valid X11 KeyCode values */
    xcb_keycode_t min_keycode = xcb_get_setup(conn)->min_keycode;
    xcb_keycode_t max_keycode = xcb_get_setup(conn)->max_keycode;

    /* Retrieve the current keyboard mapping */
    xcb_get_keyboard_mapping_cookie_t keyboard_mapping_cookie = xcb_get_keyboard_mapping(conn, min_keycode, max_keycode - min_keycode + 1);
    xcb_get_keyboard_mapping_reply_t* keyboard_mapping = xcb_get_keyboard_mapping_reply(conn, keyboard_mapping_cookie, nullptr);
    xcb_keysym_t* keyboard_keysyms = xcb_get_keyboard_mapping_keysyms(keyboard_mapping);
    int keysym_length = xcb_get_keyboard_mapping_keysyms_length(keyboard_mapping);

    for (int kc=0; kc<keysym_length; kc+=keyboard_mapping->keysyms_per_keycode) {
        for (int k=0; k<keyboard_mapping->keysyms_per_keycode; k++) {
            xcb_keysym_t ks = keyboard_keysyms[kc + k];

            if (ks == XCB_NO_SYMBOL) continue;

            /* Some modifiers switch my azerty layout into qwerty, so we
             * prioritize keysyms that are mapped to themselves. */

            if (ks == keyboard_keysyms[kc])
                keysym_mapping[ks] = ks;
            else {
                if (keysym_mapping.find(ks) == keysym_mapping.end()) {
                    keysym_mapping[ks] = keyboard_keysyms[kc];
                }
            }
        }
    }

    free(keyboard_mapping);
}

keysym_t KeyMappingXcb::nativeToKeysym(int keycode)
{
    /* Convert native virtual key to the keysym obtained without modifiers */
    if (keysym_mapping.find(keycode) != keysym_mapping.end()) {
        return keysym_mapping[keycode];
    }

    return keycode;
}

void KeyMappingXcb::default_inputs()
{
    input_mapping.clear();

    /* Map all keycode to their respective keysym. The other keysyms are unmapped. */

    /* Gather the list of valid X11 KeyCode values */
    xcb_keycode_t min_keycode = xcb_get_setup(conn)->min_keycode;
    xcb_keycode_t max_keycode = xcb_get_setup(conn)->max_keycode;

    for (int k=min_keycode; k<=max_keycode; k++) {
        xcb_keysym_t ks = xcb_key_symbols_get_keysym(keysyms, k, 0);
        if (ks == XCB_NO_SYMBOL) continue;

        for (auto iter : input_list[INPUTLIST_KEYBOARD_LATIN])
            if (iter.which == ks) {
                input_mapping[iter.which] = iter;
                break;
            }
            
        for (auto iter : input_list[INPUTLIST_KEYBOARD_MISC])
            if (iter.which == ks) {
                input_mapping[iter.which] = iter;
                break;
            }

    }
}

void KeyMappingXcb::default_input(int tab, int input_index)
{
    /* Input selected */
    SingleInput si = input_list[tab][input_index];

    /* Remove previous mapping from this key */
    for (auto iter : input_mapping) {
        if (iter.second == si) {
            input_mapping.erase(iter.first);
            break;
        }
    }

    /* Check if there's a keycode mapped to this keysym */
    if (si.type == SingleInput::IT_KEYBOARD) {

        /* Gather the list of valid X11 KeyCode values */
        xcb_keycode_t min_keycode = xcb_get_setup(conn)->min_keycode;
        xcb_keycode_t max_keycode = xcb_get_setup(conn)->max_keycode;

        for (int k=min_keycode; k<=max_keycode; k++) {
            xcb_keysym_t ks = xcb_key_symbols_get_keysym(keysyms, k, 0);
            if (ks == si.which) {
                input_mapping[si.which] = si;
                break;
            }
        }
    }
}

bool KeyMappingXcb::grabPointer(xcb_window_t window)
{
    if (blank_cursor == XCB_NONE) {
        /* Pixmaps start with undefined contents, so clear the mask explicitly */
        xcb_window_t root = xcb_setup_roots_iterator(xcb_get_setup(conn)).data->root;
        xcb_pixmap_t pixmap = xcb_generate_id(conn);
        xcb_create_pixmap(conn, 1, pixmap, root, 1, 1);

        xcb_gcontext_t gc = xcb_generate_id(conn);
        const uint32_t foreground = 0;
        xcb_create_gc(conn, gc, pixmap, XCB_GC_FOREGROUND, &foreground);
        const xcb_rectangle_t rectangle = {0, 0, 1, 1};
        xcb_poly_fill_rectangle(conn, pixmap, gc, 1, &rectangle);
        xcb_free_gc(conn, gc);

        blank_cursor = xcb_generate_id(conn);
        xcb_create_cursor(conn, blank_cursor, pixmap, pixmap, 0, 0, 0, 0, 0, 0, 0, 0);
        xcb_free_pixmap(conn, pixmap);
    }

    /* The cursor is part of the grab, so the server restores the one of the
     * window when the grab ends, including when we exit or crash. */
    xcb_grab_pointer_cookie_t cookie = xcb_grab_pointer(conn, 1, window, 0,
        XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, window, blank_cursor, XCB_CURRENT_TIME);
    xcb_grab_pointer_reply_t* reply = xcb_grab_pointer_reply(conn, cookie, nullptr);
    bool success = reply && (reply->status == XCB_GRAB_STATUS_SUCCESS);
    free(reply);
    return success;
}

void KeyMappingXcb::capturePointer(uint32_t window, bool capture)
{
    if (!is_xwayland)
        return;

    if (!capture) {
        if (captured_window != XCB_NONE) {
            xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
            xcb_flush(conn);
            captured_window = XCB_NONE;
        }
        return;
    }

    if ((window == XCB_NONE) || (captured_window == window))
        return;

    xcb_get_geometry_cookie_t geometry_cookie = xcb_get_geometry(conn, window);
    xcb_get_geometry_reply_t* geometry_reply = xcb_get_geometry_reply(conn, geometry_cookie, nullptr);
    if (geometry_reply && grabPointer(window)) {
        captured_window = window;

        /* Start from the center, otherwise the distance between the pointer
         * and the center would become the first motion of the game. */
        xcb_warp_pointer(conn, XCB_NONE, window, 0, 0, 0, 0, geometry_reply->width/2, geometry_reply->height/2);
    }
    free(geometry_reply);
}

void KeyMappingXcb::warpPointer(xcb_window_t window, int width, int height)
{
    /* While the pointer is not held (frame advance while paused, where it must
     * stay free), the position given by a warp is still used when the lock
     * ends, so that a short capture moves the real pointer. */
    bool short_capture = is_xwayland && (captured_window != window);
    if (short_capture && !grabPointer(window))
        return;

    xcb_warp_pointer(conn, XCB_NONE, window, 0, 0, 0, 0, width/2, height/2);

    if (short_capture)
        xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);

    /* The warp must reach the server now. Otherwise, it is sent with the next
     * request, just before the pointer is read for the next frame, which would
     * erase all the motion made during this frame. */
    xcb_flush(conn);
}

void KeyMappingXcb::buildAllInputs(AllInputs& ai, uint32_t window, SharedConfig& sc, bool mouse_warp)
{
    int i,j;
    int keysym_i = 0;

    ai.clear();

    /* Don't get inputs if the game window is closed */
    if (window == 0) {
        return;
    }

    /* We make all xcb queries here to save a bit of time */

    xcb_generic_error_t* error = nullptr;

    /* Get keyboard inputs */
    xcb_query_keymap_cookie_t keymap_cookie = xcb_query_keymap(conn);

    /* Get mouse inputs */
    xcb_query_pointer_cookie_t pointer_cookie;
    xcb_get_geometry_cookie_t geometry_cookie;
    if (sc.mouse_support) {
        pointer_cookie = xcb_query_pointer(conn, window);
        if (sc.mouse_mode_relative) {
            geometry_cookie = xcb_get_geometry(conn, window);
        }
    }

    xcb_query_keymap_reply_t* keymap_reply = xcb_query_keymap_reply(conn, keymap_cookie, &error);

    if (error) {
        // std::cerr << "Could not get keymap, X error" << error->error_code << std::endl;
        free(keymap_reply);
        free(error);
        return;
    }

    unsigned char* keyboard_state = keymap_reply->keys;
    keysym_t modifiers = build_modifiers(keyboard_state);

    for (i=0; i<32; i++) {
        if (keyboard_state[i] == 0)
            continue;
        for (j=0; j<8; j++) {
            if ((keyboard_state[i] >> j) & 0x1) {

                /* We got a pressed keycode */
                xcb_keycode_t kc = (i << 3) | j;
                /* Translating to keysym */
                xcb_keysym_t ks = xcb_key_symbols_get_keysym(keysyms, kc, 0);

                /* Check if we are dealing with a hotkey with or without modifiers */
                if (hotkey_mapping.find(ks) != hotkey_mapping.end()) {
                    /* Dealing with a hotkey, skipping */
                    continue;
                }

                if (modifiers) {
                    xcb_keysym_t ksm = ks | modifiers;
                    if (hotkey_mapping.find(ksm) != hotkey_mapping.end()) {
                        /* Dealing with a hotkey, skipping */
                        continue;
                    }
                }

                /* Checking the mapped input for that key */
                SingleInput si = {SingleInput::IT_NONE,0};
                if (input_mapping.find(ks) != input_mapping.end())
                    si = input_mapping[ks];

                if (si.type == SingleInput::IT_NONE) {
                    /* Key is mapped to nothing */
                    continue;
                }

                if (si.type == SingleInput::IT_KEYBOARD) {
                    /* Checking the current number of keys */
                    if (keysym_i >= AllInputs::MAXKEYS) {
                        fprintf(stderr, "Reached maximum number of inputs (%d).", AllInputs::MAXKEYS);
                        continue;
                    }

                    /* Saving the key */
                    ai.keyboard[keysym_i++] = si.which;
                }

                if (si.type == SingleInput::IT_FLAG) {
                    if (!ai.misc)
                        ai.misc.reset(new MiscInputs{});
                    
                    ai.misc->flags |= (1 << si.which);
                }

                if (sc.mouse_support) {
                    if (si.type == SingleInput::IT_POINTER_BUTTON) {
                        if (!ai.pointer)
                            ai.pointer.reset(new MouseInputs{});
                        
                        ai.pointer->mask |= (0x1u << si.which);
                    }
                }

                if (si.inputTypeIsController()) {
                    /* Key is mapped to a game controller */

                    int controller_i = si.inputTypeToControllerNumber();

                    /* Check if we support this joystick */
                    if (controller_i >= sc.nb_controllers)
                        continue;

                    if (!ai.controllers[controller_i])
                        ai.controllers[controller_i].reset(new ControllerInputs{});
                        
                    /* We don't support mapping to axes */
                    if (!si.inputTypeToAxisFlag()) {
                        ai.controllers[controller_i]->buttons |= (1 << si.which);
                    }
                }
            }
        }
    }

    free(keymap_reply);

    if (sc.mouse_support) {
        /* Get the pointer position and mask */
        xcb_query_pointer_reply_t* pointer_reply = xcb_query_pointer_reply(conn, pointer_cookie, &error);

        if (error) {
            // std::cerr << "Could not get keymap, X error" << error->error_code << std::endl;
            free(pointer_reply);
            free(error);
            return;
        }

        if (!ai.pointer)
            ai.pointer.reset(new MouseInputs{});

        ai.pointer->mode = sc.mouse_mode_relative?SingleInput::POINTER_MODE_RELATIVE:SingleInput::POINTER_MODE_ABSOLUTE;
        if (sc.mouse_mode_relative) {
            xcb_get_geometry_reply_t* geometry_reply = xcb_get_geometry_reply(conn, geometry_cookie, &error);
            if (error) {
                std::cerr << "Could not get geometry, X error" << error->error_code << std::endl;
                free(geometry_reply);
                free(error);
                return;
            }
            ai.pointer->x = pointer_reply->win_x - geometry_reply->width/2;
            ai.pointer->y = pointer_reply->win_y - geometry_reply->height/2;

            /* Warp pointer if needed */
            if (mouse_warp)
                warpPointer(window, geometry_reply->width, geometry_reply->height);

            free(geometry_reply);
        }
        else {
            ai.pointer->x = pointer_reply->win_x;
            ai.pointer->y = pointer_reply->win_y;
        }

        /* We only care about the five mouse buttons */
        if (pointer_reply->mask & XCB_BUTTON_MASK_1)
            ai.pointer->mask |= (0x1u << SingleInput::POINTER_B1);
        if (pointer_reply->mask & XCB_BUTTON_MASK_2)
            ai.pointer->mask |= (0x1u << SingleInput::POINTER_B2);
        if (pointer_reply->mask & XCB_BUTTON_MASK_3)
            ai.pointer->mask |= (0x1u << SingleInput::POINTER_B3);
        if (pointer_reply->mask & XCB_BUTTON_MASK_4)
            ai.pointer->mask |= (0x1u << SingleInput::POINTER_B4);
        if (pointer_reply->mask & XCB_BUTTON_MASK_5)
            ai.pointer->mask |= (0x1u << SingleInput::POINTER_B5);

        free(pointer_reply);
    }
}
