// Compile with `i686-w64-mingw32-gcc -mwindows cliptest.c -lopengl32 -lgdi32 -o cliptest.exe`
//
// Test game for cursor confinement under Wine and libTAS.
//
// A 640x480 borderless OpenGL window at 0,0 that confines the cursor with
// ClipCursor() to a 320x240 rectangle in its middle, like games that capture
// the mouse. It sets the clip again every 30 frames, as Windows drops it when
// the window is not the foreground one, and draws what the game sees in the
// window: GetCursorPos(), the client coordinates of the last WM_MOUSEMOVE,
// GetClipCursor() and the number of clicks. libTAS rewrites the window title
// and a GUI program has no stdout, so read the text from a screenshot (for
// example with `tesseract`) while moving the real pointer with
// `xdotool mousemove --sync X Y`.
//
// Usage: cliptest.exe [clip|none|release]
//   clip     confine the cursor from the start (default)
//   none     never call ClipCursor()
//   release  confine, then ClipCursor(NULL) after 300 frames

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

static int wm_x = -1, wm_y = -1, clicks = 0;

static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    switch (m) {
    case WM_MOUSEMOVE:
        wm_x = (short)LOWORD(l);
        wm_y = (short)HIWORD(l);
        return 0;
    case WM_LBUTTONDOWN:
        clicks++;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
    const char *mode = (cmd && *cmd) ? cmd : "clip";

    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndproc;
    wc.hInstance = inst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "cliptest";
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, "cliptest", "cliptest", WS_POPUP | WS_VISIBLE,
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

    /* Bitmap font for the text drawn in the window */
    HFONT font = CreateFontA(30, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Courier New");
    SelectObject(dc, font);
    wglUseFontBitmapsA(dc, 32, 96, 1000);

    RECT clip = { 160, 120, 480, 360 };
    if (strncmp(mode, "none", 4) != 0)
        ClipCursor(&clip);

    setvbuf(stdout, NULL, _IONBF, 0);
    int frames = 0;
    MSG msg;
    for (;;) {
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return 0;
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        /* Games that confine the mouse set the clip again regularly, as
         * Windows drops it when the window is not the foreground one */
        if (frames % 30 == 0) {
            if (strncmp(mode, "none", 4) != 0)
                ClipCursor(&clip);
            SetForegroundWindow(hwnd);
        }
        if (strncmp(mode, "release", 7) == 0 && frames >= 300)
            ClipCursor(NULL);

        POINT pt;
        RECT cr;
        GetCursorPos(&pt);
        GetClipCursor(&cr);

        char title[200];
        snprintf(title, sizeof(title),
                 "CT cur=%ld,%ld clip=%ld,%ld,%ld,%ld wm=%d,%d clicks=%d f=%d",
                 (long)pt.x, (long)pt.y, (long)cr.left, (long)cr.top,
                 (long)cr.right, (long)cr.bottom, wm_x, wm_y, clicks, frames);
        SetWindowTextA(hwnd, title);

        {
            char l1[80], l2[80];
            snprintf(l1, sizeof(l1), "cur %ld,%ld wm %d,%d", (long)pt.x, (long)pt.y, wm_x, wm_y);
            snprintf(l2, sizeof(l2), "clip %ld,%ld,%ld,%ld ck %d", (long)cr.left, (long)cr.top,
                     (long)cr.right, (long)cr.bottom, clicks);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glColor3f(1.0f, 1.0f, 1.0f);
            glListBase(1000 - 32);
            glRasterPos2f(-0.97f, 0.60f);
            glCallLists((GLsizei)strlen(l1), GL_UNSIGNED_BYTE, l1);
            glRasterPos2f(-0.97f, 0.40f);
            glCallLists((GLsizei)strlen(l2), GL_UNSIGNED_BYTE, l2);
            SwapBuffers(dc);
        }
        frames++;
    }
}
