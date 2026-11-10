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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Hyprland gives the position with a command on its IPC socket. It closes the
 * connection after each answer. Never wait for it longer than this. */
static const int HYPRLAND_TIMEOUT_MS = 10;

class HyprlandPointer : public GlobalPointer {
public:
    explicit HyprlandPointer(const std::string& path) : socket_path(path) {}

    const char* name() const override { return "Hyprland IPC"; }

    bool query(int& x, int& y) override
    {
        int new_x, new_y;
        if (request(new_x, new_y)) {
            last_x = new_x;
            last_y = new_y;
            known = true;
        }
        x = last_x;
        y = last_y;
        return known;
    }

private:
    bool request(int& x, int& y) const
    {
        int fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
        if (fd < 0)
            return false;

        sockaddr_un addr = {};
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);

        static const char command[] = "/cursorpos";
        char answer[64];
        size_t size = 0;
        bool success = (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0) &&
            (send(fd, command, sizeof(command) - 1, MSG_NOSIGNAL) == sizeof(command) - 1);

        while (success && (size < sizeof(answer) - 1)) {
            pollfd pfd = {fd, POLLIN, 0};
            if (poll(&pfd, 1, HYPRLAND_TIMEOUT_MS) <= 0) {
                success = false;
                break;
            }
            ssize_t ret = recv(fd, answer + size, sizeof(answer) - 1 - size, 0);
            if (ret < 0)
                success = false;
            if (ret <= 0)
                break;
            size += ret;
        }
        close(fd);

        if (!success)
            return false;
        answer[size] = '\0';
        return std::sscanf(answer, "%d, %d", &x, &y) == 2;
    }

    std::string socket_path;
    bool known = false;
    int last_x = 0, last_y = 0;
};

std::unique_ptr<GlobalPointer> createHyprlandPointer()
{
    const char* runtime_dir = std::getenv("XDG_RUNTIME_DIR");
    const char* signature = std::getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (!runtime_dir || !signature)
        return nullptr;

    std::string path = std::string(runtime_dir) + "/hypr/" + signature + "/.socket.sock";
    if (path.size() >= sizeof(sockaddr_un::sun_path))
        return nullptr;

    return std::make_unique<HyprlandPointer>(path);
}
