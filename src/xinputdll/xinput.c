/* The xinput dlls that libtas loads in Windows games instead of the ones of
 * Wine, so that games get the controllers of libtas.
 *
 * Wine gets controllers from a device bus (winebus.sys) that runs in another
 * process, which libtas does not hook and which is not part of savestates, so
 * what a game reads depends on the timing of that process. libtas writes the
 * state of its emulated controllers for the current frame into a file at each
 * frame boundary (see xinputshared.h), and the functions below read it, so
 * that a game gets the same state during a whole frame, and the same on each
 * playback of a movie.
 *
 * One source for all the versions of the dll: the exports of each one are in
 * a .def file, as old games import the functions by ordinal. */

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wine/xinputshared.h"

#ifndef ERROR_EMPTY
#define ERROR_EMPTY 4306
#endif

#define XUSER_INDEX_ANY 0x000000FF

#define XINPUT_DEVTYPE_GAMEPAD 0x01
#define XINPUT_DEVSUBTYPE_GAMEPAD 0x01
#define XINPUT_CAPS_FFB_SUPPORTED 0x0001
#define XINPUT_FLAG_GAMEPAD 0x00000001

#define BATTERY_DEVTYPE_GAMEPAD 0x00
#define BATTERY_TYPE_WIRED 0x01
#define BATTERY_LEVEL_FULL 0x03

typedef struct {
    WORD wButtons;
    BYTE bLeftTrigger;
    BYTE bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
} XINPUT_GAMEPAD;

typedef struct {
    DWORD dwPacketNumber;
    XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE;

typedef struct {
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
} XINPUT_VIBRATION;

typedef struct {
    BYTE Type;
    BYTE SubType;
    WORD Flags;
    XINPUT_GAMEPAD Gamepad;
    XINPUT_VIBRATION Vibration;
} XINPUT_CAPABILITIES;

typedef struct {
    XINPUT_CAPABILITIES Capabilities;
    WORD VendorId;
    WORD ProductId;
    WORD VersionNumber;
    WORD unknown1;
    DWORD unknown2;
} XINPUT_CAPABILITIES_EX;

typedef struct {
    BYTE BatteryType;
    BYTE BatteryLevel;
} XINPUT_BATTERY_INFORMATION;

typedef struct {
    WORD VirtualKey;
    WCHAR Unicode;
    WORD Flags;
    BYTE UserIndex;
    BYTE HidCode;
} XINPUT_KEYSTROKE;

/* Logs go to the file of the virtual Steam client, with its log level, as
 * Windows GUI programs have no stderr. libtas gives us both. */
#define LL_ERROR 1
#define LL_WARN 2
#define LL_INFO 3
#define LL_DEBUG 4
#define LL_TRACE 5

static int log_level = -1;
static FILE *log_file;

static void xlog(int level, const char *fmt, ...)
{
    if (log_level < 0) {
        const char *l = getenv("LIBTAS_STEAM_LOG_LEVEL");
        const char *path = getenv("LIBTAS_STEAM_LOG_FILE");
        log_level = l ? atoi(l) : LL_WARN;
        log_file = path ? fopen(path, "a") : NULL;
    }

    if ((level > log_level) || !log_file)
        return;

    va_list args;
    va_start(args, fmt);
    fprintf(log_file, "[libTAS xinput] ");
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
    va_end(args);
    fflush(log_file);
}

static INIT_ONCE shm_once = INIT_ONCE_STATIC_INIT;
static const volatile struct XInputShm *shm;

/* Convert the unix path that libtas gives us into a Windows path */
static BOOL dos_path(const char *unix_path, char *out, size_t size)
{
    typedef WCHAR *(CDECL *wine_get_dos_file_name_t)(const char *);
    wine_get_dos_file_name_t get_dos_file_name = (wine_get_dos_file_name_t)(void *)
        GetProcAddress(GetModuleHandleA("kernel32.dll"), "wine_get_dos_file_name");

    if (get_dos_file_name) {
        WCHAR *wpath = get_dos_file_name(unix_path);
        if (wpath) {
            int len = WideCharToMultiByte(CP_UTF8, 0, wpath, -1, out, (int)size, NULL, NULL);
            HeapFree(GetProcessHeap(), 0, wpath);
            if (len > 0)
                return TRUE;
        }
    }

    /* Wine maps the root directory to drive Z: by default */
    if (strlen(unix_path) + 3 > size)
        return FALSE;
    snprintf(out, size, "Z:%s", unix_path);
    for (char *c = out; *c; c++)
        if (*c == '/')
            *c = '\\';
    return TRUE;
}

static BOOL CALLBACK map_shm(PINIT_ONCE once, PVOID param, PVOID *context)
{
    const char *unix_path = getenv(LIBTAS_XINPUT_SHM_ENV);
    char path[MAX_PATH * 4];

    if (!unix_path || !dos_path(unix_path, path, sizeof(path))) {
        xlog(LL_ERROR, "No file of controllers (%s is not set), controllers will not work", LIBTAS_XINPUT_SHM_ENV);
        return TRUE;
    }

    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        xlog(LL_ERROR, "Could not open %s (error %lu), controllers will not work", path, GetLastError());
        return TRUE;
    }

    HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);
    CloseHandle(file);
    if (!mapping) {
        xlog(LL_ERROR, "Could not map %s (error %lu), controllers will not work", path, GetLastError());
        return TRUE;
    }

    const struct XInputShm *view = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, sizeof(struct XInputShm));
    CloseHandle(mapping);
    if (!view) {
        xlog(LL_ERROR, "Could not map a view of %s (error %lu), controllers will not work", path, GetLastError());
        return TRUE;
    }

    if ((view->magic != LIBTAS_XINPUT_SHM_MAGIC) || (view->version != LIBTAS_XINPUT_SHM_VERSION)) {
        xlog(LL_ERROR, "%s is not a file of controllers (magic %08x, version %u)", path, view->magic, view->version);
        UnmapViewOfFile(view);
        return TRUE;
    }

    xlog(LL_INFO, "Reading the controllers from %s", path);
    shm = view;
    return TRUE;
}

/* Copy a pad as it was at one point of the frame boundary. Returns FALSE if
 * the controller is not connected. */
static BOOL read_pad(DWORD index, struct XInputShmPad *pad)
{
    InitOnceExecuteOnce(&shm_once, map_shm, NULL, NULL);

    if (!shm || (index >= LIBTAS_XINPUT_MAX_PADS))
        return FALSE;

    for (;;) {
        uint32_t seq = shm->sequence;
        if (seq & 1)
            continue; /* libtas is writing */
        MemoryBarrier();
        *pad = shm->pads[index];
        MemoryBarrier();
        if (seq == shm->sequence)
            break;
    }

    return pad->connected != 0;
}

/* Find the controller that a call is about. XUSER_INDEX_ANY is the first
 * connected one. */
static DWORD find_pad(DWORD index, struct XInputShmPad *pad)
{
    if (index == XUSER_INDEX_ANY) {
        for (DWORD i = 0; i < LIBTAS_XINPUT_MAX_PADS; i++)
            if (read_pad(i, pad))
                return ERROR_SUCCESS;
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    if (index >= LIBTAS_XINPUT_MAX_PADS)
        return ERROR_BAD_ARGUMENTS;

    return read_pad(index, pad) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;
}

static DWORD get_state(DWORD index, XINPUT_STATE *state, BOOL with_guide)
{
    struct XInputShmPad pad;
    DWORD ret;

    if (!state)
        return ERROR_BAD_ARGUMENTS;

    ret = find_pad(index, &pad);
    if (ret != ERROR_SUCCESS) {
        static LONG not_connected_logged;
        if (!InterlockedExchange(&not_connected_logged, 1))
            xlog(LL_DEBUG, "XInputGetState(%lu): returns %lu", (unsigned long)index, (unsigned long)ret);
        return ret;
    }

    static DWORD logged_packet[LIBTAS_XINPUT_MAX_PADS] = { ~0u, ~0u, ~0u, ~0u };
    if ((index < LIBTAS_XINPUT_MAX_PADS) && (logged_packet[index] != pad.packet)) {
        logged_packet[index] = pad.packet;
        xlog(LL_DEBUG, "XInputGetState%s(%lu): packet %lu buttons %04x triggers %u,%u sticks %d,%d %d,%d",
             with_guide ? "Ex" : "", (unsigned long)index, (unsigned long)pad.packet, pad.buttons,
             pad.left_trigger, pad.right_trigger, pad.thumb_lx, pad.thumb_ly, pad.thumb_rx, pad.thumb_ry);
    }

    state->dwPacketNumber = pad.packet;
    state->Gamepad.wButtons = with_guide ? pad.buttons : (pad.buttons & ~LIBTAS_XINPUT_GUIDE);
    state->Gamepad.bLeftTrigger = pad.left_trigger;
    state->Gamepad.bRightTrigger = pad.right_trigger;
    state->Gamepad.sThumbLX = pad.thumb_lx;
    state->Gamepad.sThumbLY = pad.thumb_ly;
    state->Gamepad.sThumbRX = pad.thumb_rx;
    state->Gamepad.sThumbRY = pad.thumb_ry;
    return ERROR_SUCCESS;
}

DWORD WINAPI XInputGetState(DWORD index, XINPUT_STATE *state)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetState");
    return get_state(index, state, FALSE);
}

DWORD WINAPI XInputGetStateEx(DWORD index, XINPUT_STATE *state)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetStateEx");
    return get_state(index, state, TRUE);
}

/* Rumble goes nowhere: a TAS does not feel it */
DWORD WINAPI XInputSetState(DWORD index, XINPUT_VIBRATION *vibration)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputSetState");
    struct XInputShmPad pad;

    if (!vibration)
        return ERROR_BAD_ARGUMENTS;
    return find_pad(index, &pad);
}

static void fill_capabilities(XINPUT_CAPABILITIES *caps)
{
    memset(caps, 0, sizeof(*caps));
    caps->Type = XINPUT_DEVTYPE_GAMEPAD;
    caps->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;
    caps->Flags = XINPUT_CAPS_FFB_SUPPORTED;
    caps->Gamepad.wButtons = 0xF3FF;
    caps->Gamepad.bLeftTrigger = 0xFF;
    caps->Gamepad.bRightTrigger = 0xFF;
    caps->Gamepad.sThumbLX = (SHORT)0x7FFF;
    caps->Gamepad.sThumbLY = (SHORT)0x7FFF;
    caps->Gamepad.sThumbRX = (SHORT)0x7FFF;
    caps->Gamepad.sThumbRY = (SHORT)0x7FFF;
    caps->Vibration.wLeftMotorSpeed = 0xFF;
    caps->Vibration.wRightMotorSpeed = 0xFF;
}

DWORD WINAPI XInputGetCapabilities(DWORD index, DWORD flags, XINPUT_CAPABILITIES *caps)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetCapabilities");
    struct XInputShmPad pad;
    DWORD ret;

    if (!caps || (flags & ~XINPUT_FLAG_GAMEPAD))
        return ERROR_BAD_ARGUMENTS;

    ret = find_pad(index, &pad);
    if (ret != ERROR_SUCCESS)
        return ret;

    fill_capabilities(caps);
    return ERROR_SUCCESS;
}

/* Capabilities and the ids of the device, in xinput1_4.dll only */
DWORD WINAPI XInputGetCapabilitiesEx(DWORD unknown, DWORD index, DWORD flags, XINPUT_CAPABILITIES_EX *caps)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetCapabilitiesEx");
    struct XInputShmPad pad;
    DWORD ret;

    if (!caps || flags)
        return ERROR_BAD_ARGUMENTS;

    ret = find_pad(index, &pad);
    if (ret != ERROR_SUCCESS)
        return ret;

    memset(caps, 0, sizeof(*caps));
    fill_capabilities(&caps->Capabilities);
    caps->VendorId = 0x045e; /* Microsoft */
    caps->ProductId = 0x028e; /* Xbox 360 controller */
    return ERROR_SUCCESS;
}

void WINAPI XInputEnable(BOOL enable)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputEnable");
    /* The inputs of libtas do not depend on the focus of the game */
}

DWORD WINAPI XInputGetBatteryInformation(DWORD index, BYTE devtype, XINPUT_BATTERY_INFORMATION *info)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetBatteryInformation");
    struct XInputShmPad pad;
    DWORD ret;

    if (!info)
        return ERROR_BAD_ARGUMENTS;

    ret = find_pad(index, &pad);
    if (ret != ERROR_SUCCESS)
        return ret;

    info->BatteryType = BATTERY_TYPE_WIRED;
    info->BatteryLevel = BATTERY_LEVEL_FULL;
    return ERROR_SUCCESS;
}

DWORD WINAPI XInputGetKeystroke(DWORD index, DWORD reserved, XINPUT_KEYSTROKE *keystroke)
{
    static LONG called;
    if (!InterlockedExchange(&called, 1))
        xlog(LL_DEBUG, "first call of XInputGetKeystroke");
    struct XInputShmPad pad;
    DWORD ret;

    if (!keystroke || reserved)
        return ERROR_BAD_ARGUMENTS;

    ret = find_pad(index, &pad);
    if (ret != ERROR_SUCCESS)
        return ret;

    /* Keystrokes would be generated from the changes of the state */
    return ERROR_EMPTY;
}

DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD index, GUID *render, GUID *capture)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD WINAPI XInputGetAudioDeviceIds(DWORD index, LPWSTR render, UINT *render_count, LPWSTR capture, UINT *capture_count)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

/* Not in the dlls of Wine: games and test programs can look for it to know
 * which dll they got */
DWORD WINAPI LibTASXInputBridge(void)
{
    return 1;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(instance);
    return TRUE;
}
