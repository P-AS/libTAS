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

#include "xinput.h"
#include "xinputshared.h"

#include "logging.h"
#include "global.h"
#include "GlobalState.h"
#include "inputs/inputs.h"
#include "../shared/inputs/SingleInput.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace libtas {

namespace WineXInput {

/* -2: not looked for yet, -1: no file */
static int shm_fd = -2;

/* What is in the file */
static XInputShm written;
static bool has_written = false;

static int16_t negate(int16_t v)
{
    return (v == INT16_MIN) ? INT16_MAX : static_cast<int16_t>(-v);
}

static uint8_t toTrigger(int16_t v)
{
    return (v <= 0) ? 0 : static_cast<uint8_t>(v >> 7);
}

static uint16_t toButtons(unsigned short b)
{
    static const struct { int button; uint16_t xinput; } map[] = {
        { SingleInput::BUTTON_A, LIBTAS_XINPUT_A },
        { SingleInput::BUTTON_B, LIBTAS_XINPUT_B },
        { SingleInput::BUTTON_X, LIBTAS_XINPUT_X },
        { SingleInput::BUTTON_Y, LIBTAS_XINPUT_Y },
        { SingleInput::BUTTON_BACK, LIBTAS_XINPUT_BACK },
        { SingleInput::BUTTON_GUIDE, LIBTAS_XINPUT_GUIDE },
        { SingleInput::BUTTON_START, LIBTAS_XINPUT_START },
        { SingleInput::BUTTON_LEFTSTICK, LIBTAS_XINPUT_LEFT_THUMB },
        { SingleInput::BUTTON_RIGHTSTICK, LIBTAS_XINPUT_RIGHT_THUMB },
        { SingleInput::BUTTON_LEFTSHOULDER, LIBTAS_XINPUT_LEFT_SHOULDER },
        { SingleInput::BUTTON_RIGHTSHOULDER, LIBTAS_XINPUT_RIGHT_SHOULDER },
        { SingleInput::BUTTON_DPAD_UP, LIBTAS_XINPUT_DPAD_UP },
        { SingleInput::BUTTON_DPAD_DOWN, LIBTAS_XINPUT_DPAD_DOWN },
        { SingleInput::BUTTON_DPAD_LEFT, LIBTAS_XINPUT_DPAD_LEFT },
        { SingleInput::BUTTON_DPAD_RIGHT, LIBTAS_XINPUT_DPAD_RIGHT },
    };

    uint16_t ret = 0;
    for (const auto& m : map)
        if (b & (1u << m.button))
            ret |= m.xinput;
    return ret;
}

static void openFile()
{
    shm_fd = -1;

    const char* path = getenv(LIBTAS_XINPUT_SHM_ENV);
    if (!path)
        return;

    /* The file is not a savefile, and our open() must not track it */
    GlobalNative gn;
    shm_fd = open(path, O_RDWR | O_CLOEXEC);
    if (shm_fd < 0) {
        LOG(LL_ERROR, LCF_WINE | LCF_JOYSTICK, "Could not open %s, controllers will not work", path);
        return;
    }

    LOG(LL_DEBUG, LCF_WINE | LCF_JOYSTICK, "Sharing the controllers through %s", path);
}

static void writeAt(off_t offset, const void* data, size_t size)
{
    GlobalNative gn;
    ssize_t ret = pwrite(shm_fd, data, size, offset);
    if (ret != static_cast<ssize_t>(size))
        LOG(LL_ERROR, LCF_WINE | LCF_JOYSTICK, "Could not write the controllers (%zd of %zu bytes)", ret, size);
}

void update()
{
    if (shm_fd == -2)
        openFile();
    if (shm_fd < 0)
        return;

    XInputShm shm;
    memset(&shm, 0, sizeof(shm));
    shm.magic = LIBTAS_XINPUT_SHM_MAGIC;
    shm.version = LIBTAS_XINPUT_SHM_VERSION;
    shm.pad_count = std::min<int>(Global::shared_config.nb_controllers, LIBTAS_XINPUT_MAX_PADS);

    for (uint32_t i = 0; i < shm.pad_count; i++) {
        const ControllerInputs& ci = Inputs::game_ai.controllers[i];
        XInputShmPad& pad = shm.pads[i];

        pad.connected = 1;
        pad.buttons = toButtons(ci.buttons);
        pad.left_trigger = toTrigger(ci.axes[SingleInput::AXIS_TRIGGERLEFT]);
        pad.right_trigger = toTrigger(ci.axes[SingleInput::AXIS_TRIGGERRIGHT]);
        pad.thumb_lx = ci.axes[SingleInput::AXIS_LEFTX];
        pad.thumb_ly = negate(ci.axes[SingleInput::AXIS_LEFTY]); /* SDL has down positive */
        pad.thumb_rx = ci.axes[SingleInput::AXIS_RIGHTX];
        pad.thumb_ry = negate(ci.axes[SingleInput::AXIS_RIGHTY]);

        /* The packet number changes when the state changes */
        pad.packet = has_written ? written.pads[i].packet : 0;
        if (!has_written || (memcmp(&pad, &written.pads[i], sizeof(pad)) != 0))
            pad.packet++;
    }

    /* Write even when nothing changed: after loading a savestate, what we
     * remember is the state of that time, not what is in the file. */
    /* Sequence lock: the dll skips the pads while the sequence is odd */
    uint32_t sequence = has_written ? written.sequence : 0;
    sequence++;
    writeAt(offsetof(XInputShm, sequence), &sequence, sizeof(sequence));
    writeAt(offsetof(XInputShm, pad_count), &shm.pad_count, sizeof(shm.pad_count));
    writeAt(offsetof(XInputShm, pads), shm.pads, sizeof(shm.pads));
    sequence++;
    writeAt(offsetof(XInputShm, sequence), &sequence, sizeof(sequence));

    shm.sequence = sequence;
    written = shm;
    has_written = true;
}

}

}
