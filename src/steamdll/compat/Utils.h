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

/* Replacement of the libtas utility header for the Windows steam_api.dll */

#ifndef LIBTAS_STEAMDLL_UTILS_H_INCLUDED
#define LIBTAS_STEAMDLL_UTILS_H_INCLUDED

#include <cstddef>
#include <unistd.h>

namespace libtas {
namespace Utils
{
    /* Fails or does entire write (returns count) */
    ssize_t writeAll(int fd, const void *buf, size_t count);

    /* Fails, succeeds, or partial read due to EOF (returns num read) */
    ssize_t readAll(int fd, void *buf, size_t count);
}
}

#endif
