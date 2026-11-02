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


#include "LogOutput.h"
#include "Context.h"
#include "../shared/SharedConfig.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace LogOutput {

std::filesystem::path directory(const Context *context)
{
    return context->config.datadir / "logs";
}

std::filesystem::path newFilePath(const Context *context)
{
    std::string name = context->gamename;
    std::replace(name.begin(), name.end(), '/', '_');

    time_t now = time(nullptr);
    struct tm tm;
    localtime_r(&now, &tm);
    char stamp[32];
    strftime(stamp, sizeof(stamp), "%Y-%m-%d_%H-%M-%S", &tm);

    return directory(context) / (name + "-" + stamp + ".log");
}

static void redirectToNull()
{
    int fd = open("/dev/null", O_RDWR, S_IRUSR | S_IWUSR);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

static bool writeAll(int fd, const char *buf, ssize_t size)
{
    while (size > 0) {
        ssize_t ret = write(fd, buf, size);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            return false;
        }
        buf += ret;
        size -= ret;
    }
    return true;
}

/* Body of the helper process: copy the pipe to the console and to the file
 * until every writer is gone. */
static void teeLoop(int in, int file, int console)
{
    /* Let the game write its last messages even if we are interrupted, and do
     * not die if the console goes away. */
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    /* Do not keep any other file (the sockets of the program) open */
    long max_fd = std::min<long>(sysconf(_SC_OPEN_MAX), 4096);
    for (int fd = 3; fd < max_fd; fd++)
        if (fd != in && fd != file && fd != console)
            close(fd);

    char buf[65536];
    bool console_ok = true;
    for (;;) {
        ssize_t n = read(in, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR)
                continue;
            break;
        }
        if (n == 0)
            break;
        writeAll(file, buf, n);
        if (console_ok)
            console_ok = writeAll(console, buf, n);
    }
    _exit(0);
}

void redirectStderr(const Context *context)
{
    const bool console = context->config.sc.logging_console;
    const bool file = context->config.sc.logging_file;

    if (!console && !file) {
        redirectToNull();
        return;
    }

    if (!file)
        return; /* stderr stays on the console */

    std::error_code ec;
    std::filesystem::create_directories(directory(context), ec);
    const std::filesystem::path path = newFilePath(context);
    int log_fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (log_fd < 0) {
        std::cerr << "Could not create the log file " << path << ": " << strerror(errno) << std::endl;
        if (!console)
            redirectToNull();
        return;
    }
    std::cout << "Logging to file: " << path << std::endl;

    if (!console) {
        dup2(log_fd, STDERR_FILENO);
        close(log_fd);
        return;
    }

    /* Both: the game writes to a pipe, and a helper copies it to the console
     * and to the file. The helper is forked twice so that it is not a child
     * that nobody waits for. */
    int fds[2];
    if (pipe(fds) < 0) {
        dup2(log_fd, STDERR_FILENO);
        close(log_fd);
        return;
    }

    int console_fd = dup(STDERR_FILENO);
    pid_t pid = fork();
    if (pid < 0) {
        close(fds[0]);
        close(fds[1]);
        close(console_fd);
        dup2(log_fd, STDERR_FILENO);
        close(log_fd);
        return;
    }
    if (pid == 0) {
        if (fork() != 0)
            _exit(0);
        teeLoop(fds[0], log_fd, console_fd);
    }
    waitpid(pid, nullptr, 0);

    close(fds[0]);
    close(log_fd);
    close(console_fd);
    dup2(fds[1], STDERR_FILENO);
    close(fds[1]);
}

}
