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

/* Entry point of the steam_api.dll that libtas loads in Windows games
 * instead of their own when the virtual Steam client is enabled, because
 * the Steam API of a Windows game cannot be hooked from the unix side. It is
 * built from the same sources as the virtual Steam client of native games,
 * with the replacement headers of the compat directory. */

#include "logging.h"
#include "global.h"
#include "Utils.h"
#include "inputs/inputs.h"
#include "../library/steam/isteamuser/isteamuser.h"
#include "../library/steam/isteamremotestorage/isteamremotestorage.h"

#include <windows.h>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace libtas {

SharedConfig Global::shared_config = [] {
    SharedConfig sc;
    sc.virtual_steam = true;
    return sc;
}();

AllInputsFlat Inputs::game_ai;

static HMODULE steamdll_module = nullptr;
static char original_path[MAX_PATH];
static LogLevel log_level = LL_WARN;

void steamdll_log(LogLevel ll, const char* file, int line, const char* fmt, ...)
{
    if (ll > log_level)
        return;

    /* GUI programs have no stderr under Wine, so libtas gives us a file */
    static FILE* out = [] {
        const char* path = getenv("LIBTAS_STEAM_LOG_FILE");
        FILE* f = path ? fopen(path, "a") : nullptr;
        return f ? f : stderr;
    }();

    fprintf(out, "[libTAS steam_api] %s: ", LL_NAMES[ll]);
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
    fprintf(out, "\n");
    fflush(out);
}

ssize_t Utils::writeAll(int fd, const void *buf, size_t count)
{
    const char *ptr = static_cast<const char*>(buf);
    size_t num_written = 0;
    while (num_written < count) {
        int rc = write(fd, ptr + num_written, count - num_written);
        if (rc < 0) {
            if (errno == EINTR)
                continue;
            return rc;
        }
        if (rc == 0)
            break;
        num_written += rc;
    }
    return num_written;
}

ssize_t Utils::readAll(int fd, void *buf, size_t count)
{
    char *ptr = static_cast<char*>(buf);
    size_t num_read = 0;
    while (num_read < count) {
        int rc = read(fd, ptr + num_read, count - num_read);
        if (rc < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (rc == 0)
            break;
        num_read += rc;
    }
    return num_read;
}

const char* SteamGetOriginalLibraryPath()
{
    return original_path[0] ? original_path : nullptr;
}

/* Convert a unix path given by libTAS into a Windows path */
static std::string dosPath(const char* unix_path)
{
    typedef WCHAR* (CDECL *wine_get_dos_file_name_t)(const char*);
    wine_get_dos_file_name_t wine_get_dos_file_name = reinterpret_cast<wine_get_dos_file_name_t>(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "wine_get_dos_file_name")));

    if (wine_get_dos_file_name) {
        WCHAR* wpath = wine_get_dos_file_name(unix_path);
        if (wpath) {
            char path[MAX_PATH*4];
            int len = WideCharToMultiByte(CP_UTF8, 0, wpath, -1, path, sizeof(path), nullptr, nullptr);
            HeapFree(GetProcessHeap(), 0, wpath);
            if (len > 0)
                return path;
        }
    }

    /* Wine maps the root directory to drive Z: by default */
    std::string path = "Z:";
    path += unix_path;
    for (char& c : path)
        if (c == '/')
            c = '\\';
    return path;
}

static void init()
{
    const char* level = getenv("LIBTAS_STEAM_LOG_LEVEL");
    if (level)
        log_level = atoi(level);

    /* The loader named our module after the dll of the game */
    if (!GetModuleFileNameA(steamdll_module, original_path, sizeof(original_path)))
        original_path[0] = '\0';
    LOG(LL_DEBUG, LCF_STEAM, "Replacing %s", original_path);

    const char* userdir = getenv("LIBTAS_STEAM_USER_DATA_PATH");
    if (userdir)
        SteamSetUserDataFolder(dosPath(userdir));

    const char* remotedir = getenv("LIBTAS_STEAM_REMOTE_STORAGE");
    if (remotedir)
        SteamSetRemoteStorageFolder(dosPath(remotedir));
}

}

void *dlsym(void *handle, const char *symbol)
{
    return reinterpret_cast<void*>(GetProcAddress(libtas::steamdll_module, symbol));
}

extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        libtas::steamdll_module = instance;
        DisableThreadLibraryCalls(instance);
        libtas::init();
    }
    return TRUE;
}
