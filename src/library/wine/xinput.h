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

#ifndef LIBTAS_WINEXINPUT_H_INCLUDED
#define LIBTAS_WINEXINPUT_H_INCLUDED

namespace libtas {

/* Gives the controllers of libtas to the xinput dlls that it loads in Windows
 * games, through a file (see xinputshared.h). Does nothing unless the program
 * gave us that file. */
namespace WineXInput {

/* Write the state of the controllers for the frame that starts. Call it each
 * time that the game inputs are updated. */
void update();

}

}

#endif
