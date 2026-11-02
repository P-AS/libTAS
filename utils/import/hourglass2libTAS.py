#!/usr/bin/env python
# This script converts tas files from Hourglass format to libTAS input file.
# Just run ./hourglass2libTAS path/to/tasfile.tas
#
# A .wtf file has a 1024-byte header, followed by one 8-byte slot per frame.
# Each slot holds up to 8 Windows virtual-key codes (VK_*). The list of keys
# ends at the first zero byte. 0x01 (VK_LBUTTON) is the left mouse button.
# 0xFF is skipped. 0xF0 is also skipped: it appears in front of zero padding
# and is not a game key. 0xF2, 0xF3 and 0xF4 are not mapped and are reported
# as warnings; in the Japanese-layout files seen so far they are probably IME
# keys, which the game does not use.

import sys
import os
from collections import Counter

# Windows virtual-key code -> X11 keysym (hex, as libTAS writes them)
VK_TO_KEYSYM = {
    0x08: "ff08",  # VK_BACK        -> BackSpace
    0x09: "ff09",  # VK_TAB         -> Tab
    0x0D: "ff0d",  # VK_RETURN      -> Return
    0x10: "ffe1",  # VK_SHIFT       -> Shift_L
    0x11: "ffe3",  # VK_CONTROL     -> Control_L
    0x12: "ffe9",  # VK_MENU (Alt)  -> Alt_L
    0x13: "ff13",  # VK_PAUSE       -> Pause
    0x14: "ffe5",  # VK_CAPITAL     -> Caps_Lock
    0x1B: "ff1b",  # VK_ESCAPE      -> Escape
    0x20: "20",    # VK_SPACE       -> space
    0x21: "ff55",  # VK_PRIOR       -> Page_Up
    0x22: "ff56",  # VK_NEXT        -> Page_Down
    0x23: "ff57",  # VK_END         -> End
    0x24: "ff50",  # VK_HOME        -> Home
    0x25: "ff51",  # VK_LEFT        -> Left
    0x26: "ff52",  # VK_UP          -> Up
    0x27: "ff53",  # VK_RIGHT       -> Right
    0x28: "ff54",  # VK_DOWN        -> Down
    0x2D: "ff63",  # VK_INSERT      -> Insert
    0x2E: "ffff",  # VK_DELETE      -> Delete
    0xBF: "2f",    # VK_OEM_2 (/)   -> slash
    0xA0: "ffe1",  # VK_LSHIFT      -> Shift_L
    0xA1: "ffe2",  # VK_RSHIFT      -> Shift_R
    0xA2: "ffe3",  # VK_LCONTROL    -> Control_L
    0xA3: "ffe4",  # VK_RCONTROL    -> Control_R
    0xA4: "ffe9",  # VK_LMENU       -> Alt_L
    0xA5: "ffea",  # VK_RMENU       -> Alt_R
}
# VK_0..VK_9 are 0x30-0x39, VK_A..VK_Z are 0x41-0x5A (same as ASCII)
for vk in range(0x30, 0x3A):
    VK_TO_KEYSYM[vk] = "%x" % vk
for vk in range(0x41, 0x5B):
    VK_TO_KEYSYM[vk] = "%x" % (vk + 0x20)  # lowercase letter keysyms
# VK_F1..VK_F12 are 0x70-0x7B; keysyms XK_F1..XK_F12 are 0xffbe..0xffc9
for n in range(12):
    VK_TO_KEYSYM[0x70 + n] = "%x" % (0xffbe + n)

VK_END = 0xF0
VK_LBUTTON = 0x01
SLOT_SIZE = 8
HEADER_SIZE = 1024

if len(sys.argv) < 2:
    sys.exit("Usage: " + sys.argv[0] + " path/to/tasfile.wtf")

unknown = Counter()

with open(sys.argv[1], 'rb') as input_file:
    data = input_file.read()

body = data[HEADER_SIZE:]

output_path = os.path.join(os.path.dirname(os.path.abspath(sys.argv[1])), 'inputs')
with open(output_path, 'w') as output_file:
    for offset in range(0, len(body), SLOT_SIZE):
        slot = body[offset:offset + SLOT_SIZE]
        keysyms = []
        left_click = False
        for vk in slot:
            if vk in (0x00, VK_END, 0xFF):
                # Padding, end-of-list marker, or not a game key
                continue
            if vk == VK_LBUTTON:
                left_click = True
                continue
            keysym = VK_TO_KEYSYM.get(vk)
            if keysym is None:
                unknown[vk] += 1
                continue
            # Several VKs can map to one keysym (e.g. VK_SHIFT and VK_LSHIFT);
            # write each keysym only once per frame
            if keysym not in keysyms:
                keysyms.append(keysym)

        # Write the constructed input line
        line = '|K' + ':'.join(keysyms) + '|'
        if left_click:
            # The wtf does not store the pointer position, so it is pinned to 0:0
            line += 'M0:0:A:1....|'

        output_file.write(line + '\n')

for vk, count in sorted(unknown.items()):
    print("Warning: unmapped VK code 0x%02X (%d occurrences) skipped" % (vk, count),
          file=sys.stderr)
