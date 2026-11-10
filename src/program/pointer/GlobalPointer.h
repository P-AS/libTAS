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

#ifndef LIBTAS_GLOBALPOINTER_H_INCLUDED
#define LIBTAS_GLOBALPOINTER_H_INCLUDED

#include <memory>

/* Under Wayland, an X client can't know where the pointer is when it is not on
 * one of the X11 windows: Xwayland keeps reporting the last known position.
 * There is no common way to get it, so each class implementing this interface
 * asks a compositor in its own way. */
class GlobalPointer {
public:
    /* Where to read the pointer position from (`mouse_outside_source` setting) */
    enum Source {
        SOURCE_NONE = 0,
        SOURCE_AUTO = 1, /* Any of the following that works, except the screencast */
        SOURCE_IMAGE_CAPTURE = 2,
        SOURCE_KWIN = 3,
        SOURCE_HYPRLAND = 4,
        SOURCE_SCREENCAST = 5,
    };

    virtual ~GlobalPointer() = default;

    /* Get the last known position in the coordinates of the compositor, without
     * ever blocking the frame for long. Returns false if it is not known. */
    virtual bool query(int& x, int& y) = 0;

    virtual const char* name() const = 0;

    /* Start reading the position. Returns nullptr if that source is not available. */
    static std::unique_ptr<GlobalPointer> create(int source);
};

std::unique_ptr<GlobalPointer> createImageCapturePointer();
std::unique_ptr<GlobalPointer> createHyprlandPointer();
std::unique_ptr<GlobalPointer> createKWinPointer();
std::unique_ptr<GlobalPointer> createScreenCastPointer();

#endif
