// Compile with `i686-w64-mingw32-gcc -mwindows xinputtest.c -lopengl32 -lgdi32 -o xinputtest.exe`
// (or `x86_64-w64-mingw32-gcc` for a 64-bit program)
//
// Test game for XInput controllers under Wine and libTAS.
//
// A 640x480 OpenGL window that polls XInputGetState() for the four
// controllers each frame, and draws what the game gets: the return value,
// the packet number, the buttons, the triggers and the sticks. It also tells
// which XInput dll it got, the one of libTAS (it has an extra export,
// LibTASXInputBridge) or the one of Wine. libTAS rewrites the window title
// and a GUI program has no stdout, so read the text from a screenshot (for
// example with `tesseract`).
//
// Usage: xinputtest.exe [dll] [ordinal]
//   dll      name of the dll to load: xinput1_3 (default), xinput1_4,
//            xinput1_2, xinput1_1 or xinput9_1_0
//   ordinal  import the functions by ordinal, like old games do (Sonic
//            Generations imports XINPUT1_3.dll ordinals 2 and 3)

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    WORD wButtons;
    BYTE bLeftTrigger;
    BYTE bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
} PAD;

typedef struct {
    DWORD dwPacketNumber;
    PAD Gamepad;
} STATE;

typedef struct {
    BYTE Type;
    BYTE SubType;
    WORD Flags;
    PAD Gamepad;
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
} CAPS;

typedef struct {
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
} VIBRATION;

typedef DWORD (WINAPI *GetStateFn)(DWORD, STATE *);
typedef DWORD (WINAPI *SetStateFn)(DWORD, VIBRATION *);
typedef DWORD (WINAPI *GetCapsFn)(DWORD, DWORD, CAPS *);

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
    char dllname[64] = "xinput1_3.dll";
    int by_ordinal = 0;

    if (cmd && *cmd) {
        char arg[128];
        strncpy(arg, cmd, sizeof(arg) - 1);
        arg[sizeof(arg) - 1] = 0;
        char *tok = strtok(arg, " ");
        while (tok) {
            if (strncmp(tok, "ordinal", 7) == 0)
                by_ordinal = 1;
            else if (strncmp(tok, "xinput", 6) == 0)
                snprintf(dllname, sizeof(dllname), "%s.dll", tok);
            tok = strtok(NULL, " ");
        }
    }

    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = inst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "xinputtest";
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, "xinputtest", "xinputtest", WS_POPUP | WS_VISIBLE,
                                0, 0, 640, 480, NULL, NULL, inst, NULL);
    HDC dc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);

    HFONT font = CreateFontA(22, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Courier New");
    SelectObject(dc, font);
    wglUseFontBitmapsA(dc, 32, 96, 1000);

    HMODULE mod = LoadLibraryA(dllname);
    GetStateFn get_state = NULL;
    SetStateFn set_state = NULL;
    GetCapsFn get_caps = NULL;
    int bridge = 0;
    if (mod) {
        /* Ordinals of XInputGetState and XInputSetState in xinput1_3.dll */
        get_state = (GetStateFn)GetProcAddress(mod, by_ordinal ? (LPCSTR)2 : "XInputGetState");
        set_state = (SetStateFn)GetProcAddress(mod, by_ordinal ? (LPCSTR)3 : "XInputSetState");
        get_caps = (GetCapsFn)GetProcAddress(mod, "XInputGetCapabilities");
        bridge = GetProcAddress(mod, "LibTASXInputBridge") != NULL;
    }

    char head[80];
    snprintf(head, sizeof(head), "%s %s %s", dllname, mod ? (bridge ? "LIBTAS" : "WINE") : "NOLOAD",
             by_ordinal ? "ord" : "name");

    unsigned frame = 0;
    MSG msg;
    for (;;) {
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return 0;
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);
        glListBase(1000 - 32);

        float y = 0.88f;
        glRasterPos2f(-0.97f, y);
        glCallLists((GLsizei)strlen(head), GL_UNSIGNED_BYTE, head);

        for (DWORD i = 0; i < 4; i++) {
            STATE st;
            memset(&st, 0, sizeof(st));
            DWORD r = get_state ? get_state(i, &st) : 0xFFFF;

            char l1[80], l2[80];
            snprintf(l1, sizeof(l1), "P%lu rc=%lu pk=%lu b=%04X t=%u,%u", (unsigned long)i,
                     (unsigned long)r, (unsigned long)st.dwPacketNumber, st.Gamepad.wButtons,
                     st.Gamepad.bLeftTrigger, st.Gamepad.bRightTrigger);
            snprintf(l2, sizeof(l2), "  L=%d,%d R=%d,%d", st.Gamepad.sThumbLX, st.Gamepad.sThumbLY,
                     st.Gamepad.sThumbRX, st.Gamepad.sThumbRY);
            y -= 0.17f;
            glRasterPos2f(-0.97f, y);
            glCallLists((GLsizei)strlen(l1), GL_UNSIGNED_BYTE, l1);
            y -= 0.12f;
            glRasterPos2f(-0.97f, y);
            glCallLists((GLsizei)strlen(l2), GL_UNSIGNED_BYTE, l2);

            /* Rumble requests must not break anything */
            if (set_state) {
                VIBRATION v = { (WORD)((frame % 60) * 1000), 0 };
                set_state(i, &v);
            }
        }

        if (get_caps) {
            CAPS caps;
            memset(&caps, 0, sizeof(caps));
            DWORD r = get_caps(0, 0, &caps);
            char l[80];
            snprintf(l, sizeof(l), "caps0 rc=%lu type=%u sub=%u fl=%04X", (unsigned long)r, caps.Type,
                     caps.SubType, caps.Flags);
            y -= 0.17f;
            glRasterPos2f(-0.97f, y);
            glCallLists((GLsizei)strlen(l), GL_UNSIGNED_BYTE, l);
        }

        SwapBuffers(dc);
        frame++;
    }
}
