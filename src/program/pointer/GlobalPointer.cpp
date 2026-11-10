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

#include "GlobalPointer.h"

std::unique_ptr<GlobalPointer> GlobalPointer::create(int source)
{
    switch (source) {
        case SOURCE_IMAGE_CAPTURE:
            return createImageCapturePointer();
        case SOURCE_KWIN:
            return createKWinPointer();
        case SOURCE_HYPRLAND:
            return createHyprlandPointer();
        case SOURCE_SCREENCAST:
            return createScreenCastPointer();
        case SOURCE_AUTO: {
            /* Each of these checks that it runs on the right compositor. The
             * screencast is left out, as it has a real cost and users may be
             * notified that their screen is shared, so it must be asked for. */
            std::unique_ptr<GlobalPointer> pointer = createHyprlandPointer();
            if (!pointer)
                pointer = createImageCapturePointer();
            if (!pointer)
                pointer = createKWinPointer();
            return pointer;
        }
        default:
            return nullptr;
    }
}
