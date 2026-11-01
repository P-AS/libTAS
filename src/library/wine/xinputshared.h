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

#ifndef LIBTAS_XINPUTSHARED_H_INCLUDED
#define LIBTAS_XINPUTSHARED_H_INCLUDED

/* Controller state shared between libtas and the xinput dlls that it loads in
 * Windows games instead of the ones of Wine. Wine serves controllers through
 * a device bus in another process, which libtas does not control and which is
 * not saved in savestates, so libtas writes the state of its emulated
 * controllers for the current frame into a file at each frame boundary, and
 * the dll maps that file.
 *
 * Plain C, because both the unix library and the Windows dlls include it. */

#include <stdint.h>

#define LIBTAS_XINPUT_SHM_MAGIC 0x50494e58u /* "XNIP" */
#define LIBTAS_XINPUT_SHM_VERSION 1
#define LIBTAS_XINPUT_MAX_PADS 4

/* Environment variable with the unix path of the file, set by the program */
#define LIBTAS_XINPUT_SHM_ENV "LIBTAS_XINPUT_SHM"

/* Bits of XINPUT_GAMEPAD.wButtons */
#define LIBTAS_XINPUT_DPAD_UP 0x0001
#define LIBTAS_XINPUT_DPAD_DOWN 0x0002
#define LIBTAS_XINPUT_DPAD_LEFT 0x0004
#define LIBTAS_XINPUT_DPAD_RIGHT 0x0008
#define LIBTAS_XINPUT_START 0x0010
#define LIBTAS_XINPUT_BACK 0x0020
#define LIBTAS_XINPUT_LEFT_THUMB 0x0040
#define LIBTAS_XINPUT_RIGHT_THUMB 0x0080
#define LIBTAS_XINPUT_LEFT_SHOULDER 0x0100
#define LIBTAS_XINPUT_RIGHT_SHOULDER 0x0200
#define LIBTAS_XINPUT_GUIDE 0x0400 /* only given by XInputGetStateEx() */
#define LIBTAS_XINPUT_A 0x1000
#define LIBTAS_XINPUT_B 0x2000
#define LIBTAS_XINPUT_X 0x4000
#define LIBTAS_XINPUT_Y 0x8000

struct XInputShmPad {
    uint32_t connected;
    uint32_t packet; /* changes each time the state changes */
    uint16_t buttons;
    uint8_t left_trigger;
    uint8_t right_trigger;
    int16_t thumb_lx;
    int16_t thumb_ly; /* up is positive, as in XInput */
    int16_t thumb_rx;
    int16_t thumb_ry;
};

struct XInputShm {
    uint32_t magic;
    uint32_t version;
    /* Sequence lock: odd while libtas is writing the pads */
    volatile uint32_t sequence;
    uint32_t pad_count;
    struct XInputShmPad pads[LIBTAS_XINPUT_MAX_PADS];
};

#endif
