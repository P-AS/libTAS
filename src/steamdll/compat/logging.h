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

/* Replacement of the libtas logging header for the Windows steam_api.dll */

#ifndef LIBTAS_STEAMDLL_LOGGING_H_INCLUDED
#define LIBTAS_STEAMDLL_LOGGING_H_INCLUDED

#include "../../shared/lcf.h"

namespace libtas {

/* Print a message on stderr if the level is enabled with the
 * LIBTAS_STEAM_LOG_LEVEL environment variable (default: warnings) */
void steamdll_log(LogLevel ll, const char* file, int line, const char* fmt, ...)
    __attribute__((format(printf, 4, 5)));

}

#define LOG(ll, lcf, ...) libtas::steamdll_log(ll, __FILE__, __LINE__, __VA_ARGS__)
#define LOGTRACE(lcf, ...) libtas::steamdll_log(LL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOGTRACE_SIMPLE(lcf) LOGTRACE(lcf, "%s call.", __func__)

#endif
