#!/usr/bin/env python
# This script converts tas files from Hourglass format to libTAS input file.
# Just run ./hourglass2libTAS path/to/tasfile.tas

import re
import sys
import os
import math

input_file = open(sys.argv[1], 'rb')
output_file = open(os.path.dirname(sys.argv[1])+'/inputs' , 'w')

input_keys = [0x0D, 0xA0, 0xA2, 0x1B, 0x20, 0x25, 0x26, 0x27, 0x28, 0x10, 0x11, 0x09, 0x2E] + \
    list(range(0x30, 0x3A)) + list(range(0x41, 0x5B))
mapped_keys = ["ff0d", "ffe1", "ffe3", "ff1b", "20", "ff51", "ff52", "ff53", "ff54", "ffe1", "ffe3", "ff09", "ffff"] + \
    ["30", "31", "32", "33", "34", "35", "36", "37", "38", "39"] + \
    ["61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f"] + \
    ["70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b"]

input_file.seek(1024, 0)

byte = input_file.read(1)
while byte != b"":
    keyboard_state = ''
    left_click = False
    for k in range(8):
        i = 0
        if (byte[0] == 0x01):
            # VK_LBUTTON, left mouse button
            left_click = True
        # 0xFF is not a game key, skip it
        elif (byte[0] != 0 and byte[0] != 0xFF):
            try:
                i = input_keys.index(byte[0])
            except ValueError:
                print("Unknown VK key " + str(byte[0]))
            else:
                keyboard_state += mapped_keys[i] + ':'
        byte = input_file.read(1)

    # Remove trailing ':'
    if keyboard_state:
        keyboard_state = keyboard_state[:-1]

    # Write the constructed input line
    line = '|K' + keyboard_state + '|'
    if left_click:
        # The wtf does not store the pointer position, so it is pinned to 0:0
        line += 'M0:0:A:1....|'

    output_file.write(line + '\n')

input_file.close()
output_file.close()
