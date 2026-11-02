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


#ifndef LIBTAS_LOGOUTPUT_H_INCLUDED
#define LIBTAS_LOGOUTPUT_H_INCLUDED

#include <filesystem>

/* Forward declaration */
struct Context;

/* Where the logs of the game go: the console and/or a log file */
namespace LogOutput {

    /* Folder of the log files: "logs" in the data directory */
    std::filesystem::path directory(const Context *context);

    /* Path of the log file of a game launched now:
     * <logs folder>/<executable name>-YYYY-MM-DD_HH-MM-SS.log */
    std::filesystem::path newFilePath(const Context *context);

    /* Redirect stderr of the calling process, which is the child that starts
     * the game, according to the logging settings. When both the console and
     * a file are enabled, a helper process copies everything that is written
     * to stderr to both, so that they hold the same text. */
    void redirectStderr(const Context *context);
}

#endif
