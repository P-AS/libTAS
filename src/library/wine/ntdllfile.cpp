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

/* Wine opens and creates files from the wineserver process, which sends back
 * the file descriptor, so our open() hooks never see the files of Windows
 * games. Hook the file opening syscalls of the unix side of ntdll instead, and
 * give back a handle on our savefiles when preventing writing to disk. */

#include "ntdllfile.h"
#include "winehook.h"

#include "logging.h"
#include "global.h"
#include "GlobalState.h"
#include "fileio/SaveFileList.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/syscall.h>

namespace libtas {

namespace {

typedef struct {
    uint16_t Length;
    uint16_t MaximumLength;
    char16_t *Buffer;
} UNICODE_STRING;

typedef struct {
    uint32_t Length;
    void *RootDirectory;
    UNICODE_STRING *ObjectName;
    uint32_t Attributes;
    void *SecurityDescriptor;
    void *SecurityQualityOfService;
} OBJECT_ATTRIBUTES;

typedef struct {
    int64_t CreationTime;
    int64_t LastAccessTime;
    int64_t LastWriteTime;
    int64_t ChangeTime;
    uint32_t FileAttributes;
} FILE_BASIC_INFORMATION;

typedef struct {
    int64_t CreationTime;
    int64_t LastAccessTime;
    int64_t LastWriteTime;
    int64_t ChangeTime;
    int64_t AllocationSize;
    int64_t EndOfFile;
    uint32_t FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION;

typedef struct {
    union {
        uint32_t Status;
        void *Pointer;
    };
    uintptr_t Information;
} IO_STATUS_BLOCK;

/* NTSTATUS values */
const uint32_t STATUS_SUCCESS = 0;
const uint32_t STATUS_OBJECT_NAME_NOT_FOUND = 0xC0000034;
const uint32_t STATUS_OBJECT_NAME_COLLISION = 0xC0000035;

/* Create dispositions */
const uint32_t FILE_SUPERSEDE = 0;
const uint32_t FILE_OPEN = 1;
const uint32_t FILE_CREATE = 2;
const uint32_t FILE_OPEN_IF = 3;
const uint32_t FILE_OVERWRITE = 4;
const uint32_t FILE_OVERWRITE_IF = 5;

/* Values of IO_STATUS_BLOCK.Information */
const uintptr_t FILE_SUPERSEDED = 0;
const uintptr_t FILE_OPENED = 1;
const uintptr_t FILE_CREATED = 2;
const uintptr_t FILE_OVERWRITTEN = 3;

/* Create options */
const uint32_t FILE_DIRECTORY_FILE = 0x00000001;
const uint32_t FILE_DELETE_ON_CLOSE = 0x00001000;
const uint32_t FILE_OPEN_BY_FILE_ID = 0x00002000;

/* File attributes */
const uint32_t FILE_ATTRIBUTE_ARCHIVE = 0x00000020;

/* Windows time unit is 100ns, starting from 1601-01-01 */
const int64_t TICKSPERSEC = 10000000;
const int64_t SECS_1601_TO_1970 = 11644473600LL;

/* Access rights */
const uint32_t FILE_WRITE_DATA = 0x00000002;
const uint32_t FILE_APPEND_DATA = 0x00000004;
const uint32_t GENERIC_ALL = 0x10000000;
const uint32_t GENERIC_WRITE = 0x40000000;

}

namespace orig {

static uint32_t (*NtCreateFile)(void **handle, uint32_t access, OBJECT_ATTRIBUTES *attr, IO_STATUS_BLOCK *io, int64_t *alloc_size, uint32_t attributes, uint32_t sharing, uint32_t disposition, uint32_t options, void *ea_buffer, uint32_t ea_length);
static uint32_t (*NtOpenFile)(void **handle, uint32_t access, OBJECT_ATTRIBUTES *attr, IO_STATUS_BLOCK *io, uint32_t sharing, uint32_t options);
static uint32_t (*wine_server_fd_to_handle)(int fd, uint32_t access, uint32_t attributes, void **handle);
static uint32_t (*NtQueryAttributesFile)(const OBJECT_ATTRIBUTES *attr, FILE_BASIC_INFORMATION *info);
static uint32_t (*NtQueryFullAttributesFile)(const OBJECT_ATTRIBUTES *attr, FILE_NETWORK_OPEN_INFORMATION *info);

}

static std::string utf16ToUtf8(const char16_t *str, size_t len)
{
    std::string out;
    for (size_t i = 0; i < len; i++) {
        uint32_t c = str[i];
        if ((c >= 0xD800) && (c < 0xDC00) && (i + 1 < len) &&
            (str[i+1] >= 0xDC00) && (str[i+1] < 0xE000)) {
            c = 0x10000 + ((c - 0xD800) << 10) + (str[i+1] - 0xDC00);
            i++;
        }
        if (c < 0x80) {
            out += static_cast<char>(c);
        }
        else if (c < 0x800) {
            out += static_cast<char>(0xC0 | (c >> 6));
            out += static_cast<char>(0x80 | (c & 0x3F));
        }
        else if (c < 0x10000) {
            out += static_cast<char>(0xE0 | (c >> 12));
            out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (c & 0x3F));
        }
        else {
            out += static_cast<char>(0xF0 | (c >> 18));
            out += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (c & 0x3F));
        }
    }
    return out;
}

static std::string winePrefix()
{
    const char *prefix_env = getenv("WINEPREFIX");
    if (prefix_env)
        return prefix_env;

    const char *home = getenv("HOME");
    if (!home)
        return "";
    return std::string(home) + "/.wine";
}

/* Returns if the file is inside the temporary directory of a wine user.
 * Games such as Clickteam ones extract files there and look for them by
 * listing the directory, which does not show our savefiles, and these are not
 * save data anyway. */
static bool isWineTempFile(const std::string &file)
{
    static std::string usersdir;
    if (usersdir.empty()) {
        GlobalNative gn;
        std::string users = winePrefix() + "/drive_c/users";
        char *path = realpath(users.c_str(), nullptr);
        if (!path)
            return false;
        usersdir = std::string(path) + "/";
        free(path);
    }

    if (file.compare(0, usersdir.size(), usersdir) != 0)
        return false;

    /* Skip the user name */
    size_t slash = file.find('/', usersdir.size());
    if (slash == std::string::npos)
        return false;
    const char *rest = file.c_str() + slash + 1;

    return (strncasecmp(rest, "Temp/", 5) == 0) ||
           (strncasecmp(rest, "AppData/Local/Temp/", 19) == 0);
}

/* Convert a NT path of a file on a drive (\??\C:\dir\file) into a unix path,
 * looking up each component without case like wine does. Returns an empty
 * string if the path cannot be converted, or if a parent directory of the
 * file does not exist. */
static std::string ntToUnixPath(const OBJECT_ATTRIBUTES *attr)
{
    if (!attr || attr->RootDirectory || !attr->ObjectName || !attr->ObjectName->Buffer)
        return "";

    std::string ntpath = utf16ToUtf8(attr->ObjectName->Buffer, attr->ObjectName->Length / sizeof(char16_t));
    if ((ntpath.size() < 7) || (ntpath.compare(0, 4, "\\??\\") != 0) ||
        (ntpath[5] != ':') || (ntpath[6] != '\\'))
        return "";

    char drive = ntpath[4];
    if ((drive >= 'A') && (drive <= 'Z'))
        drive += 'a' - 'A';
    if ((drive < 'a') || (drive > 'z'))
        return "";

    GlobalNative gn;

    std::string prefix = winePrefix();
    if (prefix.empty())
        return "";

    std::string dosdevice = prefix + "/dosdevices/" + drive + ":";
    char *root = realpath(dosdevice.c_str(), nullptr);
    if (!root)
        return "";
    std::string unixpath(root);
    free(root);

    /* Resolve each component of the path */
    size_t pos = 7;
    while (pos < ntpath.size()) {
        size_t next = ntpath.find('\\', pos);
        bool last = (next == std::string::npos);
        std::string component = ntpath.substr(pos, last ? std::string::npos : next - pos);
        pos = last ? ntpath.size() : next + 1;

        if (component.empty() || (component == "."))
            continue;

        if (component == "..") {
            size_t slash = unixpath.rfind('/');
            if (slash != std::string::npos && slash != 0)
                unixpath.resize(slash);
            continue;
        }

        if (unixpath.empty() || (unixpath.back() != '/'))
            unixpath += '/';

        struct stat st;
        if (stat((unixpath + component).c_str(), &st) == 0) {
            unixpath += component;
            continue;
        }

        /* Look for the component without case */
        bool found = false;
        DIR *dir = opendir(unixpath.c_str());
        if (dir) {
            struct dirent *entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (strcasecmp(entry->d_name, component.c_str()) == 0) {
                    unixpath += entry->d_name;
                    found = true;
                    break;
                }
            }
            closedir(dir);
        }

        if (!found) {
            /* Only the file itself may be missing */
            if (!last)
                return "";
            unixpath += component;
        }
    }

    return unixpath;
}

static bool savefileExists(const char *file)
{
    if (SaveFileList::getSaveFileFd(file) != 0)
        return true;

    if (SaveFileList::isSaveFileRemoved(file))
        return false;

    GlobalNative gn;
    struct stat st;
    return (stat(file, &st) == 0);
}

/* Open a savefile and give a wine handle on it. Returns false if the file
 * must be opened by wine instead. */
static bool openSaveFileHandle(void **handle, uint32_t access, OBJECT_ATTRIBUTES *attr, IO_STATUS_BLOCK *io, uint32_t disposition, uint32_t options, uint32_t *status)
{
    if (!Global::shared_config.prevent_savefiles)
        return false;

    if (options & (FILE_DIRECTORY_FILE | FILE_OPEN_BY_FILE_ID))
        return false;

    if (disposition > FILE_OVERWRITE_IF)
        return false;

    std::string unixpath = ntToUnixPath(attr);
    if (unixpath.empty() || isWineTempFile(unixpath))
        return false;
    const char *file = unixpath.c_str();

    bool write = (access & (FILE_WRITE_DATA | FILE_APPEND_DATA | GENERIC_WRITE | GENERIC_ALL)) ||
                 (disposition == FILE_SUPERSEDE) || (disposition == FILE_OVERWRITE) ||
                 (disposition == FILE_OVERWRITE_IF);

    /* Deleting a file, which is done by opening it with delete-on-close */
    if (options & FILE_DELETE_ON_CLOSE) {
        if (!SaveFileList::isSaveFile(file))
            return false;

        bool exists = savefileExists(file);
        if (!exists && ((disposition == FILE_OPEN) || (disposition == FILE_OVERWRITE))) {
            *status = STATUS_OBJECT_NAME_NOT_FOUND;
            io->Status = *status;
            return true;
        }

        LOG(LL_DEBUG, LCF_FILEIO | LCF_WINE, "Remove savefile %s", file);
        SaveFileList::removeSaveFile(file);

        /* Give a handle on an anonymous file, so that the game can still use
         * it until it closes the handle */
        int fd = syscall(SYS_memfd_create, file, 0);
        if (fd < 0)
            return false;
        *status = orig::wine_server_fd_to_handle(fd, access, attr->Attributes, handle);
        NATIVECALL(close(fd));
        io->Status = *status;
        io->Information = exists ? FILE_OPENED : FILE_CREATED;
        return true;
    }

    int oflag = write ? O_RDWR : O_RDONLY;
    if (!SaveFileList::isSaveFile(file, oflag))
        return false;

    bool exists = savefileExists(file);

    switch (disposition) {
        case FILE_OPEN:
        case FILE_OVERWRITE:
            if (!exists) {
                *status = STATUS_OBJECT_NAME_NOT_FOUND;
                io->Status = *status;
                return true;
            }
            break;
        case FILE_CREATE:
            if (exists) {
                *status = STATUS_OBJECT_NAME_COLLISION;
                io->Status = *status;
                return true;
            }
            break;
        default:
            break;
    }

    if ((disposition == FILE_SUPERSEDE) || (disposition == FILE_OVERWRITE) ||
        (disposition == FILE_OVERWRITE_IF))
        oflag |= O_TRUNC;
    if ((access & FILE_APPEND_DATA) && !(access & (FILE_WRITE_DATA | GENERIC_WRITE | GENERIC_ALL)))
        oflag |= O_APPEND;
    if (!exists)
        oflag |= O_CREAT;

    LOG(LL_DEBUG, LCF_FILEIO | LCF_WINE, "Open savefile %s with access %x and disposition %d", file, access, disposition);

    int fd = SaveFileList::openSaveFile(file, oflag);
    if (fd < 0) {
        LOG(LL_ERROR, LCF_FILEIO | LCF_WINE, "Could not open savefile %s", file);
        return false;
    }

    /* Wineserver duplicates the file descriptor, and the savefile keeps its
     * own, so that the content stays in memory after the handle is closed. */
    *status = orig::wine_server_fd_to_handle(fd, access, attr->Attributes, handle);
    io->Status = *status;

    /* We don't know when wine closes its own file descriptor, so consider the
     * savefile closed now, so that removing it discards its content. */
    SaveFileList::closeSaveFile(fd);

    if (*status != STATUS_SUCCESS)
        return true;

    if (!exists)
        io->Information = FILE_CREATED;
    else if (disposition == FILE_SUPERSEDE)
        io->Information = FILE_SUPERSEDED;
    else if ((disposition == FILE_OVERWRITE) || (disposition == FILE_OVERWRITE_IF))
        io->Information = FILE_OVERWRITTEN;
    else
        io->Information = FILE_OPENED;

    return true;
}

static uint32_t NtCreateFile(void **handle, uint32_t access, OBJECT_ATTRIBUTES *attr, IO_STATUS_BLOCK *io, int64_t *alloc_size, uint32_t attributes, uint32_t sharing, uint32_t disposition, uint32_t options, void *ea_buffer, uint32_t ea_length)
{
    if (!GlobalState::isNative()) {
        LOGTRACE(LCF_FILEIO | LCF_WINE, "%s call with access %x and disposition %d", __func__, access, disposition);
        uint32_t status;
        if (openSaveFileHandle(handle, access, attr, io, disposition, options, &status))
            return status;
    }

    return orig::NtCreateFile(handle, access, attr, io, alloc_size, attributes, sharing, disposition, options, ea_buffer, ea_length);
}

static uint32_t NtOpenFile(void **handle, uint32_t access, OBJECT_ATTRIBUTES *attr, IO_STATUS_BLOCK *io, uint32_t sharing, uint32_t options)
{
    if (!GlobalState::isNative()) {
        LOGTRACE(LCF_FILEIO | LCF_WINE, "%s call with access %x", __func__, access);
        uint32_t status;
        if (openSaveFileHandle(handle, access, attr, io, FILE_OPEN, options, &status))
            return status;
    }

    return orig::NtOpenFile(handle, access, attr, io, sharing, options);
}

static int64_t toWindowsTime(const struct timespec &ts)
{
    return (ts.tv_sec + SECS_1601_TO_1970) * TICKSPERSEC + ts.tv_nsec / 100;
}

/* Get the attributes of a savefile. Returns false if the file must be
 * queried by wine instead. */
static bool statSaveFile(const OBJECT_ATTRIBUTES *attr, struct stat *st, uint32_t *status)
{
    if (!Global::shared_config.prevent_savefiles)
        return false;

    std::string unixpath = ntToUnixPath(attr);
    if (unixpath.empty())
        return false;
    const char *file = unixpath.c_str();

    if (SaveFileList::isSaveFileRemoved(file)) {
        *status = STATUS_OBJECT_NAME_NOT_FOUND;
        return true;
    }

    int fd = SaveFileList::getSaveFileFd(file);
    if (fd == 0)
        return false;

    GlobalNative gn;
    if (fstat(fd, st) != 0)
        return false;

    *status = STATUS_SUCCESS;
    return true;
}

static uint32_t NtQueryAttributesFile(const OBJECT_ATTRIBUTES *attr, FILE_BASIC_INFORMATION *info)
{
    if (!GlobalState::isNative()) {
        LOGTRACE_SIMPLE(LCF_FILEIO | LCF_WINE);
        struct stat st;
        uint32_t status;
        if (statSaveFile(attr, &st, &status)) {
            if (status == STATUS_SUCCESS) {
                info->CreationTime = toWindowsTime(st.st_mtim);
                info->LastAccessTime = toWindowsTime(st.st_atim);
                info->LastWriteTime = toWindowsTime(st.st_mtim);
                info->ChangeTime = toWindowsTime(st.st_ctim);
                info->FileAttributes = FILE_ATTRIBUTE_ARCHIVE;
            }
            return status;
        }
    }

    return orig::NtQueryAttributesFile(attr, info);
}

static uint32_t NtQueryFullAttributesFile(const OBJECT_ATTRIBUTES *attr, FILE_NETWORK_OPEN_INFORMATION *info)
{
    if (!GlobalState::isNative()) {
        LOGTRACE_SIMPLE(LCF_FILEIO | LCF_WINE);
        struct stat st;
        uint32_t status;
        if (statSaveFile(attr, &st, &status)) {
            if (status == STATUS_SUCCESS) {
                info->CreationTime = toWindowsTime(st.st_mtim);
                info->LastAccessTime = toWindowsTime(st.st_atim);
                info->LastWriteTime = toWindowsTime(st.st_mtim);
                info->ChangeTime = toWindowsTime(st.st_ctim);
                info->AllocationSize = static_cast<int64_t>(st.st_blocks) * 512;
                info->EndOfFile = st.st_size;
                info->FileAttributes = FILE_ATTRIBUTE_ARCHIVE;
            }
            return status;
        }
    }

    return orig::NtQueryFullAttributesFile(attr, info);
}

void hook_ntdll_file(const char* path)
{
    void *ntdll;
    NATIVECALL(ntdll = dlopen(path, RTLD_LAZY | RTLD_NOLOAD));
    if (!ntdll) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not load %s", path);
        return;
    }
    NATIVECALL(orig::wine_server_fd_to_handle = reinterpret_cast<decltype(orig::wine_server_fd_to_handle)>(dlsym(ntdll, "wine_server_fd_to_handle")));
    if (!orig::wine_server_fd_to_handle) {
        LOG(LL_ERROR, LCF_WINE | LCF_HOOK, "Could not find wine_server_fd_to_handle");
        return;
    }

    hook_wine_syscall(path, "NtCreateFile", reinterpret_cast<void**>(&orig::NtCreateFile), reinterpret_cast<void*>(NtCreateFile));
    hook_wine_syscall(path, "NtOpenFile", reinterpret_cast<void**>(&orig::NtOpenFile), reinterpret_cast<void*>(NtOpenFile));
    hook_wine_syscall(path, "NtQueryAttributesFile", reinterpret_cast<void**>(&orig::NtQueryAttributesFile), reinterpret_cast<void*>(NtQueryAttributesFile));
    hook_wine_syscall(path, "NtQueryFullAttributesFile", reinterpret_cast<void**>(&orig::NtQueryFullAttributesFile), reinterpret_cast<void*>(NtQueryFullAttributesFile));
}

}
