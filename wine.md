# Running Windows games through Wine

This document records what was learned while making Windows games run under
libTAS through Wine: problems found, how they were fixed, design choices, and
what is still open. Keep it up to date when working on anything Wine related
(see `AGENTS.md`).

Tested setup: Wine 11.20 (WoW64, so 32-bit games run in a 64-bit unix
process), libxcb 1.17, Mesa llvmpipe, Arch Linux, on a rootful Xwayland
display. Games used for testing:

- **I Wanna Be The Boshy** (Clickteam Fusion, 32-bit, Direct3D 9 through
  wined3d, DirectSound audio, `.ini` save files, extracts extensions into the
  Wine temporary directory). Also the Hourglass movie imported in
  `utils/import/shar-iwannabetheboshy-darkboshy.ltm`.
- **Sonic Robo Blast 2 v2.2.15** (`srb2win.exe`, 32-bit, SDL2 + OpenGL).
- A pinball game tested by Randomno (commit `75ffe274`).
- **Sonic Generations** (Steam, 32-bit, Direct3D 9), launched through Proton
  11.0 (see [Proton](#proton)).
- Small mingw test programs (file I/O, thread exit across savestates), see
  [Testing](#testing-and-debugging).

## How libTAS hooks a Wine game

### Which process gets hooked

`LD_PRELOAD` is inherited by every process Wine starts (wineserver, wineboot,
explorer, ...). `GameThread` sets:

- `LIBTAS_DELAY_INIT` so that libtas does not initialize too early;
- `LIBTAS_WINE_GAME` to the executable name, and `wine_should_init()` only
  initializes libtas in the process whose `argv[1]` is the game, and unsets
  `LD_PRELOAD` for the others;
- `LIBTAS_WINE_RESTART` on WoW64: a 32-bit game is started in a 64-bit process
  which restarts itself, so libtas is kept loaded but not initialized in the
  first process, and initializes in the restarted one (`79c46189`).

`isWineGame()` (in `wine/winehook.cpp`) tells the rest of the library that it
runs inside the Wine game process.

Launching (`a148b183`): fall back to `wine` when `wine64` is missing (WoW64
builds), choose `libtas.so` from the architecture of the Wine process rather
than the game's, and look for Proton in its `files` directory.

### Hooking the unix side of Wine

Since Wine 5, Windows dlls are PE files that we cannot hook from `LD_PRELOAD`
(the old `user32.dll.so`, `kernel32.dll.so` and `wined3d.dll.so` hooks are
dead code on recent Wine). Instead we hook the **unix side** libraries:

- `win32u.so`: user/gdi syscalls (input, message waits);
- `ntdll.so`: time, sleeps, file opening;
- libc/Xlib/EGL/ALSA calls made by `winex11.so`, `winealsa.so`, Mesa, etc. are
  caught by the regular libTAS hooks.

Two hooking methods are used:

- `hook_wine_syscall(library, name, ...)` replaces the entry of a syscall in
  the **syscall table** of the unix library. Only calls coming from the Windows
  side are redirected; Wine's own internal calls keep using the original
  function. Use this whenever Wine itself calls the function internally and
  must keep real behaviour (for example Wine computes its internal timeouts
  from the real time).
- `HOOK_PATCH_ORIG(...)` patches the function itself (used for the input
  getters in `win32u.cpp`), so internal calls are affected too.

`dlopen()` notices when `ntdll.so`, `win32u.so` and `winex11.so` are loaded
and installs the hooks / records their address ranges (`dlhook.cpp`).

### Crashes inside unix calls abort frame boundaries

When libtas code crashes (SIGSEGV) while running inside a Wine unix call (for
example inside `eglSwapBuffers` called from `x11drv_egl_surface_swap`), Wine's
`handle_syscall_fault` turns it into a Windows exception and **returns to the
PE side**, abandoning everything on the unix stack. If this happens during a
frame boundary, `frame_mutex` stays locked and the next frame deadlocks at
frame 1 or 2. Two bugs looked exactly like this:

- `FileDebug::update()` indexed `fd_history[-1]` for half-registered pipes
  (`6f1fa3f7`);
- writing to the shared user data page after Wine made it read-only
  (`a0192386`).

Symptom: the game stops at an early frame and the render thread is blocked in
`DeterministicTimer::enterFrameBoundary()` while no thread is inside a frame
boundary. Run with `WINEDEBUG=+seh` and look for `handle_syscall_fault`, then
catch the fault with gdb (see [Testing](#testing-and-debugging)).

## Time

`f3e7bbe4` made Wine time deterministic:

- `NtQueryPerformanceCounter`, `NtQuerySystemTime` and `NtDelayExecution` are
  replaced in the `ntdll.so` syscall table, so that only the game sees fake
  time.
- `GetTickCount()` and friends read the **shared user data page**
  (`KUSER_SHARED_DATA` at `0x7ffe0000`) directly, without any call. libtas
  replaces that page with a private copy (`takeover_wine_shared_data()`) and
  writes deterministic tick count, interrupt time and system time into it
  (`update_wine_shared_data()`) at frame boundaries and when adding delay.
- Wine resets the protection of that page from its own memory views, so the
  page is made writable again before each update, and taken over again if Wine
  mapped it anew (`a0192386`, found with SRB2).
- `NtDelayExecution`: infinite sleeps, `Sleep(0)` yields and alertable sleeps
  are left to Wine. `Sleep(0)` counts as a `GetTickCount` call for
  time-tracking, because busy loops on the shared page usually yield this way.

### Waiting on messages

Wine computes the deadlines of its waits from the **real** clock, while the
game computes its timeouts from our deterministic clock. Two consequences:

- `MsgWaitForMultipleObjects(timeout)` slept in real time without advancing
  our time, and time only advanced through time-tracking (1 ms per 100 QPC
  calls), so Boshy ran at one frame every ~10 s in game. The hook on
  `NtUserMsgWaitForMultipleObjectsEx` checks without waiting and transfers the
  timeout as a deterministic sleep when nothing is signaled (`0a68fde2`).
  Infinite and alertable waits are left to Wine.
- `GetMessage()` with no message available blocks forever, while the message
  comes from a timer thread (`timeSetEvent`) that waits until our time reaches
  its deadline, which nothing advances. On the main thread, the
  `NtUserGetMessage` hook advances time by 1 ms while no message is available,
  and waits 1 ms of real time in between so that other threads can post their
  message (`dc9c10e4`).

### Time-tracking

Boshy busy-waits on `QueryPerformanceCounter`, so QPC time-tracking (threshold
100 on the main thread) must be enabled, otherwise it freezes around frame
107. Time-tracking of secondary threads does not help, see the main thread
section below.

## Threads

### Main thread

wined3d presented frames from its command stream (CSMT) thread, so the frame
boundary made it the main thread, and the game logic thread (which calls
QPC) became a secondary thread without time-tracking. For Wine games, the main
thread is never switched away from the first thread (`58152c07`).
"Keep main first thread" in the debug settings does the same for any game.

### CSMT

With CSMT, the game thread queues a present to the wined3d thread and then
sleeps, which advanced time before the present happened, triggering non-draw
frames (321 out of 962 frames in Boshy) and irregular frame timings, and made
the frame boundary (and thus the checkpoint thread) alternate between two
threads, which broke savestates. `GameThread` sets `csmt=0x0` in
`WINE_D3D_CONFIG` unless the user already set `csmt` (`50c98cd1`). Frames are
then presented by the game thread.

## Rendering and windows

- On Wine 11, OpenGL/wined3d presents go through
  `win32u_wglSwapBuffers -> x11drv_egl_surface_swap -> eglSwapBuffers`, which
  our EGL hook turns into frame boundaries. The EGL surface is drawn on a
  child window of the top-level game window, created on another X connection.
- `XShmPutImage` from `winex11.so` (window surface flushes, used by GDI
  rendering) can happen from any thread that waits for messages; flushes from
  other threads than the main one are not frame boundaries (`75ffe274`).
- **Game window selection**: the game window was the last mapped top-level
  window. SRB2 maps its small startup window again after the window it draws
  to, so the program listened for hotkeys on the wrong window and inputs went
  there ("libTAS loses control after initialization"). For Wine games, we
  track the X window of each EGL window surface and promote its top-level
  window when swapping (`7424fbb3`). This is restricted to Wine games because
  on other platforms the native window is not an X window, and an Xlib error
  would kill the game (Wine installs its own X error handler).
- **Game window not registered** (Proton 11.0, Sonic Generations): windows
  were only registered when created directly under the root window, and a
  surface's top-level window could only be promoted if it was registered. The
  first registered window was wined3d's "WineD3D fake window", whose
  destruction sent window id 0 to the program, which then listened for
  hotkeys nowhere. Proton's winex11 also draws with GLX
  (`glXCreateWindow`/`glXSwapBuffers`, no EGL), which had no promotion at
  all. Now, for Wine games: a top-level window that is not override-redirect
  is registered when it is mapped (so before the first frame is drawn, which
  matters when the game is paused at start), `glXCreateWindow` records the X
  window of each GLX window like the EGL surfaces, and each swap makes the
  top-level window drawn to the game window (`XlibGameWindow::setDrawn()`),
  adding it if missing. Override-redirect windows are never added: the 1x1
  dummy parent of offscreen client windows is one (Boshy registered it as
  its game window during testing) (`70d6789d`).
- **Offscreen client surfaces**: when the GL (or Vulkan) window is a child
  window, has DPI scaling or has child windows (`needs_offscreen_rendering()`
  in winex11 `init.c`), Wine renders into a client window detached under a
  dummy parent, and after the swap `client_surface_present()` (win32u)
  blits it into the top-level window through GDI. Boshy draws into a
  627x667 child window, so it is offscreen. Our frame boundary runs inside
  `eglSwapBuffers`, before that blit, so while paused the window showed the
  previous frame, and the redraws on Expose (window moved over the game)
  only updated the offscreen window, leaving black holes. We patch
  `client_surface_present` to remember the last presented surface (holding a
  reference with `client_surface_add_ref`), and present it again after each
  swap that libTAS performs in Wine games (EGL and GLX). Wine still presents
  after the hook returns, which costs a second blit per frame but no
  measurable time. Surfaces that are not offscreen get no DC, and the present
  does nothing. To reproduce: pause, raise another window over the game with
  `wmctrl -i -a` and move it away; `xwininfo -root -tree` shows the client
  window under a 1x1 window of another connection (`gdi_display`). Wine
  ignores Expose events itself, since we filter them (`00292664`).
- **No OSD** (Sonic Generations through Proton):
  - The OSD was drawn into the framebuffer that wined3d had bound when
    presenting, not on the window. `RenderHUD_GL::render()` now binds
    framebuffer 0 for drawing and restores the binding.
  - Screen capture (used for the OSD layout, redraws while paused and video
    encodes) picked the XShm backend before the GL one, because Wine flushes
    the GDI surfaces of windows with `XShmPutImage`, and took its size
    (1280x768) from such a flush. For Wine games drawing with OpenGL, the GL
    backend is used.
  - The window manager and Wine resize windows without going through our
    hooks, so the size of the drawable is now read at each swap
    (`glXQueryDrawable`, `eglQuerySurface`) and screen capture is set up
    again with it at the next frame boundary (`ScreenCapture::setDrawnSize()`).
    Resizing our GL objects in the swap hook instead made the game spin
    forever with a black screen, at the frame where it recreates its GL
    context (cause not found).
  - The `XGetWindowAttributes` hook replaced the size of every window with
    the size of screen capture. Wine queries the geometry of all its windows,
    and draws into a child window of another size than its top-level window
    (Sonic Generations draws 1600x900 into a 1600x1000 window), so the size
    is no longer faked for Wine games.
  - The GL screen capture setup left its texture bound, while games and
    wined3d track their bindings; it is now restored.
- **Encoded video with swapped colors and a flipped strip** (Sonic Generations,
  Proton 11.0, 1280x720 window): the video was 1280x768, red and blue were
  swapped, and the bottom 48 rows showed a vertically flipped, stale image.
  The first screen capture setup happens before the game is known to use
  OpenGL (the first frames come from Wine's GDI flushes), and picked the XShm
  backend (1280x768 from the XImage of the flush, BGRX), which failed and
  stayed in `ScreenCapture::impl`, so the next try skipped the backend
  choice and used it. The encoder then built its muxer from that size and
  format. When the capture was set up again a frame later with the GL
  backend (1280x720, RGBA, `ScreenCapture::setDrawnSize()`), nothing told the
  encoder: RGBA bytes were labelled BGRX (swapped colors) and each frame of
  720 rows was read as 768, the last 48 rows being the heap behind the
  buffer. A failed setup now drops the backend, and the encoder compares the
  size and format of the capture with those of its muxer at each frame, and
  starts again if they differ (in the same file during the first 30 frames,
  as a new segment after). The video then starts a few frames (7 in the test)
  after the first frame of the game (`98eef6d7`). Found by encoding with `-d`, with
  `TMPDEBUG` logs of the backend and the drawable size at each swap.
- Wine sets the title of SRB2's window to "FluidSynth...", cosmetic.
- SRB2's default window is 2560x1415 (from its own config), larger than a small
  test display.

## Input

- `03bdd579` hooks `NtUserGetAsyncKeyState`, `NtUserGetKeyState`,
  `NtUserGetKeyboardState` and `NtUserGetCursorPos` in `win32u.so`, maps libTAS
  inputs to virtual keys (`inputs/winekeyboardlayout.cpp`), fixes right/middle
  mouse buttons (Xlib numbers them differently), and converts root-relative
  pointer warps to game window coordinates. Games polling keys (Boshy) only
  need these.
- Games reading window messages (SRB2 through SDL2) get keys from X events
  that libTAS generates for each X connection of the game. Wine opens several X
  connections (one per thread), and `XlibEventQueue::insert()` modified the
  event while looking for the receiving window, so when the first connection
  did not know the game window, the next ones received the event with a null
  window and dropped it (`23c55fd8`). This bug is not Wine specific.
- **Unfocused window, no inputs** ("if a window is not focused, it does not
  receive any input, and playback desyncs"). Two separate causes:
  - winex11 only reads X events when wineserver notices that the X
    connection of the thread is readable (it then sets the `QS_DRIVER`
    internal queue bit, and `process_driver_events()` calls
    `X11DRV_ProcessEvents()`). Events that libTAS inserts in its emulated
    Xlib queue don't touch the socket, so they waited for an unrelated real
    X event (pointer motion, focus change, ...), which mostly happens while
    the game window has the focus. Keys arrived late, in bursts, or never.
    Now the `NtUserPeekMessage`, `NtUserGetMessage` and
    `NtUserMsgWaitForMultipleObjectsEx` hooks first check the libTAS queue
    of the thread's display (`x11::threadDisplay`, the last display opened
    by the thread); if it has events, `sendXlibWakeEvent()` makes the X
    server send a dummy `_LIBTAS_WAKE_EVENTS` ClientMessage to a 1x1 window
    owned by that connection (filtered when pulled), and we call
    `orig::NtUserMsgWaitForMultipleObjectsEx(0, NULL, 1, 0, 0)` until the
    wake event has been pulled (at most 100 times). Wine processes driver
    events inside that wait, and the same `X11DRV_ProcessEvents()` call pulls
    the wake event and empties our queue (or processes nothing when nested in
    an event handler, which also ends our wait). Don't wait for the queue
    to be signaled: the processing clears `QS_DRIVER`, so with empty masks a
    long wait always ran out its full timeout, which made Boshy 3x slower.
    winex11.so and win32u.so are stripped, so `X11DRV_ProcessEvents` cannot
    be called directly, and keeping `QS_DRIVER` always set would make
    `GetMessage` busy-loop.
  - Wine 11 picks the foreground window from the root `_NET_ACTIVE_WINDOW`
    property (`net_active_window_notify()`, then
    `X11DRV_GetWindowStateUpdates()`). A non-Wine active window (the libTAS
    window, a terminal) maps to the desktop, so the game got `WM_ACTIVATE`
    inactive and lost the keyboard focus. `XGetWindowProperty` now reports
    the game window when the active window is not one of the game windows
    (`XlibGameWindow::contains()`). Without a window manager the property
    does not exist and Wine uses FocusIn/FocusOut, which we already filter.
  - Reproduced with a mingw test program logging `GetForegroundWindow()`
    and `WM_KEYDOWN` counts per frame, openbox running in the test Xwayland,
    and a Lua script focusing an xterm with `wmctrl -i -a` (xeyes does not
    accept the focus). Before: no `WM_KEYDOWN` at all even when focused, and
    the game deactivated after the xterm got the focus. After: every key
    press arrives on its frame (`d3529335`). Boshy (which polls keys) froze at frame 51
    once the xterm took the focus, because Clickteam stops when its window
    is deactivated; it now plays on, and the imported movie gives the same
    image at frame 3000 as before the change, in the same time.
- **Window manager, no keyboard at all** (Sonic Generations with openbox):
  - Wine uses the "globally active" focus model: its windows set the input
    hint to False and ask for `WM_TAKE_FOCUS`, so the window manager does not
    focus them itself but sends a message to the game, which then calls
    `XSetInputFocus`. A paused game never processes that message, so clicking
    the game window left the focus elsewhere, and no key (inputs or libTAS
    hotkeys) reached it. libTAS already removed `WM_TAKE_FOCUS` and forced the
    input hint, but only for the window that was already the game window,
    while Wine sets the hints before mapping it. For Wine games, the input
    hint is now forced for all windows.
  - `XlibGameWindow::topLevel()` walked up to the child of the root window,
    which is the frame of the window manager, so libTAS made that frame the
    game window and sent the game's key events to it, where Wine never saw
    them (no `KeyPress` in `WINEDEBUG=+event`). It now stops at the window
    that has the `WM_STATE` property (the one the window manager manages), or
    at a window already known.
  - Without a window manager both problems are hidden: tests that set the
    focus with `xdotool windowfocus` (`XSetInputFocus`) passed. Test with
    openbox and `xdotool windowactivate`.
- **Cannot click back into the game, nor on libTAS** (Sonic Generations,
  openbox): after clicking another window, clicking the game window did not
  give it the focus again, and clicks on a window below the game (libTAS)
  did nothing either. `xwininfo -root -tree` showed an InputOnly
  override-redirect window without properties, mapped over the game and
  created by the game's X connection: the window that winex11 maps over the
  clipping rectangle and passes to `XGrabPointer()` to confine the cursor
  while the game clips it. Our `XGrabPointer()` is emulated (it only reads the
  geometry of the window), and `_NET_ACTIVE_WINDOW` is reported as the game
  window while another window is active (see above), so Wine never releases
  the clipping, and the window catches the real clicks: the window manager
  does not manage it, so it never focuses anything. `xdotool windowunmap` on it
  was enough to get the focus back with one click, which found the cause. Wine
  games no longer map that window (`isWineClipWindow()` in `XMapWindow()` and
  `XMapRaised()`: InputOnly and override-redirect). 8 of 8 clicks back on the
  game worked, from the xterm and from libTAS, keys reach the game (the
  Pause hotkey stops and resumes the frames). In recording mode (`o` hotkey
  on a copy of the movie): real keys and pointer are recorded while the game
  has the focus, nothing while the xterm has it, and a key held while the
  focus leaves is not left pressed. Test with `xdotool mousemove
  --sync X Y click 1`: the real pointer of the rootful Xwayland is moved by
  the user's mouse too, which made single-click tests fail at random.
  - Checked with a test game that really confines the cursor (32-bit OpenGL
    window, `ClipCursor()` to a 320x240 rectangle set again every 30 frames,
    as Windows drops it when the window is not the foreground one; it draws
    `GetCursorPos`, `WM_MOUSEMOVE` and `GetClipCursor` as text, read back
    with `tesseract` on a screenshot, since libTAS rewrites the window title
    and a GUI program has no stdout). The game's `WM_MOUSEMOVE` coordinates
    stay inside the rectangle for 8 of 8 real pointer positions, with and
    without the fix. After `ClipCursor(NULL)`, `GetClipCursor()` is the whole
    screen again in both, and `GetCursorPos()` follows the pointer with the
    fix, but was off by (-481,-261) without it. `WM_MOUSEMOVE` coordinates
    stay clamped to the old rectangle after the release in both (cause not
    found). Needs a clean display: a killed run leaves its clip window mapped
    (`kill -9` on a Wine process left one over the test rectangle).
- **Crash when dragging the mouse** (`free(): unaligned chunk detected in
  tcache 2`, `double free or corruption`): while a button is held, Wine
  enables XInput2 raw events, and libTAS generates `XI_RawMotion` events
  whose cookie data is allocated by us and freed by our `XFreeEventData()`.
  `XlibEventQueueList::insert()` put the same event, with the same data
  pointer, in the queue of each display, and Wine opens one display per
  thread, so the data was freed several times (in `X11DRV_ProcessEvents()`).
  Each queued event now owns a copy of the data (`duplicateXIEventData()`),
  and the generated original is freed after queuing (`6406f967`). Moving the pointer or
  clicking alone did not crash; holding a button while moving did, which is
  also what moving the game window does. With `mouse_support=false` it did
  not crash either.
  - Found with a temporary wrapper of the `SIGABRT` handler that Wine
    installs (through our `sigaction` hook), printing `backtrace()` before
    calling it: Wine turns the abort into an exception and the faulting
    thread exits, so `coredumpctl info` does not show it, and `ptrace` is
    not allowed. The frames were `NtUserPeekMessage` -> `processXlibEvents`
    -> win32u -> winex11 `X11DRV_ProcessEvents` -> `XFreeEventData` ->
    `free`.
  - We also gave native X events to Dear ImGui (OSD) from whichever thread
    pulled them, while ImGui is not thread-safe and Wine reads events from
    several threads. Events pulled by other threads are now queued and given
    to ImGui at the next HUD frame of the main thread (`b3f011c5`). This was
    not the cause of the crash above.
- Hourglass import: Hourglass records `VK_LBUTTON` as a held key, which the
  importer converts into a pointer button (`95373926`).

## Audio

- libtas makes pulseaudio fail (`pa_mainloop_new()` returns NULL) so that games
  use ALSA. `winepulse` does not check this and aborts
  (`Assertion 'm' failed ... pa_mainloop_set_poll_func()`). The
  `WINEDLLOVERRIDES=winepulse.drv=d` override is ignored by Wine 11, so we
  block the `dlopen()` of `winepulse.so` instead, which makes `winepulse.drv`
  fail to load and mmdevapi fall back to `winealsa` (`ab396f1c`).
- `winealsa.so` uses `SND_PCM_FORMAT_FLOAT_LE`, interleaved, 48 kHz, which our
  ALSA hooks support.
- Boshy had no sound when preventing writing to disk: Clickteam extracts its
  sound filters (`*.sft`) into the Wine temporary directory and finds them by
  listing the directory, which does not show our in-memory savefiles. Files in
  the temporary directories of Wine users are left to Wine (`364b37c8`). To
  check whether a game plays sounds, `WINEDEBUG=+dsound` shows
  `DSOUND_MixToPrimary ... for N buffers`.
- winealsa also opens capture streams when listing devices, which we refuse
  (we have no audio input). This printed `WARN: Unsupported stream
  direction 1` three times per run; it is now an info message.

## Files and "Prevent writing to disk"

Wine opens and creates files from **wineserver**, which sends back the file
descriptor, so our `open()` hooks never see the files of Windows games
(`0dd54fac`). `wine/ntdllfile.cpp` hooks, in the `ntdll.so` syscall table:

- `NtCreateFile` / `NtOpenFile`: convert the NT path (`\??\C:\...`) into a unix
  path through `$WINEPREFIX/dosdevices`, resolving each component without case
  like Wine (`wine_nt_to_unix_file_name` is not exported by `ntdll.so`). If the
  file is a savefile, open it from our savefile list (a memfd holding a copy of
  the original) and wrap it in a Wine handle with `wine_server_fd_to_handle()`.
  Dispositions (create, open, overwrite, supersede, append) give the same
  results as Windows. The savefile is considered closed right away, as
  wineserver keeps its own duplicate of the descriptor.
- Deletion (`FILE_DELETE_ON_CLOSE`, used by `DeleteFile()`): mark the savefile
  as removed and hand out an anonymous file.
- `NtQueryAttributesFile` / `NtQueryFullAttributesFile`: report savefiles that
  only exist in memory, and removed ones as missing.

Left to Wine: directories, open by file id, paths relative to a handle,
non-drive paths, and files in the Wine temporary directories (see Audio).
Writes to `/proc` (Wine sets thread names in `/proc/self/task/<tid>/comm`) are
not savefiles (`621b5617`).

Not handled yet:

- renaming (`MoveFile`) goes through wineserver, so it acts on disk, or fails
  for files only in memory;
- directory listings don't show files that only exist in memory;
- creating directories happens on disk;
- the registry (`user.reg`, `system.reg`) is written by wineserver.

## Steam

The Steam API of a Windows game is the PE `steam_api.dll` (or
`steam_api64.dll`), and the game calls its C++ interfaces with the MSVC
ABI, so the hooks of the virtual Steam client of native games don't apply,
and 32-bit PE code cannot call our 64-bit unix code anyway.

- `src/steamdll` builds `libtas-steam_api.dll` and `libtas-steam_api64.dll`
  from the same `src/library/steam` sources with the mingw-w64 cross
  compilers (optional, skipped when they are missing) (`88d6b09a`), with replacement
  headers in `src/steamdll/compat` for `logging.h`, `hook.h`, `global.h`,
  etc. `RETURN_NATIVE` returns a default value, as there is no Steam client
  to fall back to. They are installed next to `libtas.so`.
- When the virtual Steam client is enabled, the `NtOpenFile` hook gives
  the loader our dll instead of the `steam_api.dll` or `steam_api64.dll`
  of the game (`openSteamApiReplacement()` in `wine/ntdllfile.cpp`). It
  recognizes the loader by its access `GENERIC_READ | SYNCHRONIZE` with
  `FILE_NON_DIRECTORY_FILE` (`open_dll_file()` in ntdll `loader.c`), while
  `CreateFile()` goes through `NtCreateFile`, so our dll can still read the
  original one (named by `GetModuleFileName()` of our module) to find the
  interface versions, like the native code does with `libsteam_api.so`.
- The dll cannot receive messages from the program, so the launcher passes
  `LIBTAS_STEAM_USER_DATA_PATH` and `LIBTAS_STEAM_REMOTE_STORAGE` (unix
  paths, converted with `wine_get_dos_file_name`). Remote storage files are
  then opened through `NtCreateFile`, so "Prevent writing to disk" applies.
  `LIBTAS_STEAM_LOG_LEVEL=5` prints every call on stderr (default: warnings).
- MSVC ABI differences, handled in the shared sources with macros that do
  nothing on Linux (`steamtypes.h`):
  - methods are `thiscall` on i386 (`this` in ecx, the callee pops the
    arguments): `STEAM_CALL` on the functions of the C vtables. C++
    interface classes get it from mingw g++ already.
  - overloaded virtual methods are in the reverse order of their
    declaration: `STEAM_OVERLOAD_PAIR` in the ISteamUserStats vtables
    (`GetStat`, `SetStat`, `GetUserStat`, `GetGlobalStat`, ...), and the two
    `CCallbackBase::Run()`.
  - methods returning a class (`CSteamID`) take a hidden return pointer
    after `this` and return it: `STEAMID_RET`, `STEAMID_RET_ARG...`,
    `STEAMID_RETURN`.
  - callback structs use 8-byte packing (`UserStatsReceived_t`).
  (`1ba66efd`). These were checked by disassembling the flat wrappers of Valve's MSVC
  `steam_api.dll` (`SteamAPI_ISteamUserStats_GetStat0` uses vtable slot 1
  and loads a float, `GetStat` slot 2; `SteamAPI_ISteamUser_GetSteamID`
  pushes a return pointer), then by calling our interfaces through those
  wrappers from small mingw test programs (32-bit with the Sega Classics
  dll, 64-bit with the Fancy Pants Adventures one), and dispatching a
  `UserStatsReceived_t` to a hand-made MSVC-like callback object.
- The ISteamUser and ISteamUserStats functions did not take the interface
  pointer, so the arguments of native games were shifted too; fixed for both.
- Tested first with Kero Blaster (32-bit): the game loads our dll, calls
  `SteamAPI_Init`, registers its user stats callbacks and runs normally.
  Touhou 10 from Steam does not use the Steam API at all (only thcrap
  ships a `steam_api.dll`).
- Games crashed when calling methods missing at the end of our vtables:
  ISteamFriends only had its first 12 methods, so Cave Story+ calling
  `RequestUserInformation` jumped into the next vtable (it showed as a call
  to ISteamGameServer `LogOn`, then a jump to 0). All 76 methods up to
  ISteamFriends017 are now stubs. Our C++ interface classes have a single
  layout, while some interfaces change in the middle between versions:
  ISteamUGC matches `STEAMUGC_INTERFACE_VERSION010` only (Sonic Mania,
  Shovel Knight), and Cave Story+ (version 020) crashed inside libstdc++ code.
  Other versions now get a null interface with a warning (`7a6caf53`), which Cave Story+
  handles.
- To check the vtable layout of an interface version, disassemble the flat
  wrappers `SteamAPI_<interface>_<method>` of a Valve `steam_api.dll` that
  has that version: each one loads `this` and calls a fixed vtable offset.
  Compare those slots with the declaration order of our class.
- Tested with Kero Blaster, Sonic CD and Cave Story+ (32-bit). Without the
  virtual Steam client, Sonic CD stops on "Steam client is not running".
  Cave Story+ busy-waits on `QueryPerformanceCounter` at frame 1 with or
  without Steam, and needs QPC time-tracking on the main thread (100), like
  Boshy. Native VVVVVV and Shovel Knight still work.
- The dll has no access to libtas inputs, so Steam controllers stay
  neutral.

## Proton

When the Proton path has a `proton` script, the game is started with
`proton run <game>` (WIP commit `dc47524b`), giving it the Steam app id, the
compatdata prefix and the Steam client path, unless they are already set in
the environment or the env overrides. Proton then starts the game from its
`steam.exe`.

- **WoW64**: Proton 10 has a separate WoW64 wine in `files/bin-wow64`,
  while Proton 11.0 has a single wine that runs 32-bit games in a 32-bit
  process (`lib/wine/i386-unix`) unless `PROTON_USE_WOW64=1` (which sets
  `WINEARCH=wow64`). Our 64-bit `libtas.so` could not be preloaded in the
  32-bit process (`wrong ELF class: ELFCLASS64`) and the game never connected.
  The launcher now always sets `PROTON_USE_WOW64=1` (`ed6c30f2`).
- **wined3d**: Proton draws Direct3D 9 to 11 with DXVK and vkd3d-proton
  (Vulkan). With software rendering, libTAS points `VK_DRIVER_FILES` to
  lavapipe, which is often not installed, and DXVK failed with `Failed to
  create Vulkan instance` and a C++ exception. Our Wine support is built
  around wined3d (CSMT setting, GL swaps as frame boundaries), so the launcher
  sets `PROTON_USE_WINED3D=1` unless the user set it (`ed6c30f2`).
- Proton 11.0's winex11 draws with GLX, not EGL, see "Game window not
  registered" in [Rendering and windows](#rendering-and-windows).
- `Unhandled exception in Xalia: No displays available` is printed by
  Proton's accessibility helper and is harmless. `Proton: Error while copying
  to ... amdxcffx64.dll` too.
- libtas stays preloaded but uninitialized in Proton's `steam.exe`. Its
  `XCreateWindow` hook registered the windows of that process, and
  destroying them sent the window id on a socket that does not exist there
  (`send() returns -1 with error Socket operation on non-socket` at frame 0,
  found with `strace -f -k -e trace=sendto -e status=failed`).
  `XlibGameWindow` now ignores windows handled in the native state
  (`70d6789d`).
- The launcher ran `ldd` and `readelf` on the game executable (missing
  libraries, Godot detection, symbol addresses), which printed `Not an ELF
  file` and `not a dynamic executable` for Windows games; they are skipped
  for non-ELF executables (`429835df`).
- Running Sonic Generations through Proton 11.0 (rootful Xwayland with
  openbox, its own game directory and prefix, the user's libTAS config) is
  documented for users in `docs/guides/sonic-generations.md`.
- A prefix created by another Proton (here GE-Proton 11 from a Flatpak Steam)
  has its system dlls as symlinks into that Proton (`/run/host/...`), and
  does not start with another one (`could not load kernel32.dll`). Use a new
  compatdata directory (`STEAM_COMPAT_DATA_PATH`).
- Prefix paths written as Windows paths by the prefix's own Proton (the
  `S:` drive of the Steam library in Sonic Generations' registry key) don't
  exist in a new prefix.

- **Custom screen resolution** (windowed game): with `screen_width` set,
  our XRandR hooks describe a fake monitor with output and CRTC id 1, which
  don't exist on the server. Wine reads the EDID of outputs
  (`XRRGetOutputProperty`), and the request failed with `BadRROutput`,
  killing the game. `XRRGetOutputProperty` now reports no property for the
  fake output, and `XRRGetOutputPrimary` returns it. Then, Sonic Generations
  asks for a 1280x720 fullscreen mode, and Proton's fullscreen hack keeps
  the real mode and scales the game to the size of the screen (1600x900 in
  a 1600x1000 window), so the game window had the custom size but the
  picture was cropped. With a custom screen resolution, the launcher sets
  `WINE_DISABLE_FULLSCREEN_HACK=1`: Wine then switches the display mode
  through XRandR, which we fake, and the game draws 1280x720 into a 1280x720
  window. Without a custom resolution, fullscreen games still fill the
  screen.
- That was not enough on a display larger than the custom resolution: Wine
  takes the monitor size and display modes from its `explorer.exe` process,
  where libtas is loaded but not initialized, so the game still drew a
  1600-wide picture into the 1280x720 window (cropped, centered at x=800).
  The launcher now sets `LIBTAS_WINE_SCREEN_SIZE=WxH`, `wine_should_init()`
  copies it into `screen_width`/`screen_height` in the processes that are not
  the game, and our `dlsym()` hook returns our `XRR*` functions there even in
  native mode, as winex11 loads libXrandr with `dlopen()` (`c61bd8c8`).

### Sonic Generations

Tested with Proton 11.0 (`proton-11.0-2e`): the game reaches the title
screen, keyboard input, pause, frame advance, fast-forward and savestates
work (identical image after loading). Setup needed in the prefix and the
game directory, all from the game itself rather than libTAS:

- `Game files either missing or corrupt please reinstall`: the game reads
  `HKLM\Software\Wow6432Node\Sega\Sonic Generations`, which Steam's install
  script creates: `install_path`, `exe_path`, `savelocation` and `locale`
  (`1033`). The `locale` value was the one missing (found with
  `WINEDEBUG=+reg,+msgbox`: `RegQueryValueExW` of `locale` right before
  `MSGBOX_OnInit`).
- `The detected configuration does not match your current hardware`:
  `GraphicsConfig.cfg` (CRLF lines) stores the Direct3D 9 adapter description,
  device GUID, monitor id and resolution, and must match what d3d9 reports.
  DXVK reports the real GPU (`NVIDIA GeForce RTX 2070 SUPER`, a per-device
  GUID), wined3d a card from its table (`NVIDIA GeForce RTX 2070`, or `GTX 470`
  with llvmpipe) with the fixed GUID `aeb2cdd4-6e41-43ea-941c-8361cc760781`
  and an empty monitor id. Saving in `ConfigurationTool.exe` keeps the stored
  adapter; delete `GraphicsConfig.cfg` first and save again under the same
  conditions as the game (wined3d, software GL). The resolution must also be
  one of the modes of the display (see Open problems). The last line,
  `1515474505`, is the FOURCC `INTZ`, not a checksum.
- Keyboard: `Documents/My Games/Sonic Generations/Saved Games/PlayerInput.cfg`
  had all keyboard bindings at 0 (also in the user's own prefix), so the game
  ignored keys. Its first two lines can be taken from `DefaultInput.cfg` of
  the game directory (DirectInput scancodes, Return is Start).
- With software rendering it runs at about 25 frames per second in real time.
- **Results screen without its button prompt** (frame 4866 of a movie of the
  first stage, goal post; Idle Threads and the virtual Steam client on): after
  the goal the Results screen shows time, rings, score and rank, but the "A"
  prompt to continue never appears (checked 1500 frames later, the rank still
  animates, so the game is not frozen), and the inputs of the movie do
  nothing. In `steam_api.log`, the game calls `SetAchievement` (twice for
  each of RingCollector and BeginStory) and `StoreStats` several times, and
  only `UserStatsReceived_t` (1101) comes back, with the game id hard-coded to
  105600. The virtual client never sent `UserStatsStored_t` (1102) nor
  `UserAchievementStored_t` (1103), which the real client sends after
  `StoreStats()`. Now sent, and the id comes from `SteamAppId`
  (`a24bd09f`). Verified:
  replaying the same movie at normal speed, the game leaves the Results
  screen on the movie's own button presses (frame 6208 on), loads the next
  screen ("Now loading...") and reaches the hub at frame 6532; without the
  fix it was still on Results at frame 6415. The prompt itself was not
  captured in a screenshot.

## Determinism of Windows games

What it took to make runs of the same movie play the same way in Sonic
Generations (Proton 11.0), found by tracing the main thread (`logging_level`
Trace with only the Wine, wait, sleep and time-set categories, then compare the
lines of the main thread between runs, ignoring addresses, thread ids, the
`sleep for` real-time duration and `processXlibEvents with N events`).

- **Use the virtual Steam client.** With the real one (`virtual_steam=false`),
  `Steam_BGetCallback` of `steamclient.so` (called by `SteamAPI_RunCallbacks`)
  talks to the real Steam process and does `nanosleep(1 ms)` in real time, and
  nearly every pair of runs differed by frame 11-36. A movie recorded with the
  real Steam client does not play the same with the virtual one (loadings take
  a different number of frames) and is not reproducible with the real one.
- **Steam Cloud must be reported as enabled** in the virtual Steam client,
  otherwise the game stays on its first "Saving..." screen (after the title
  screen). With the cloud disabled, the game writes `sonic.sav` to its
  folder, then waits forever (the save thread exits normally, and the game
  does not call the Steam API during the wait); the real client reports the
  cloud as enabled, and the game saves with `FileWrite("sonic.sav")` and
  never writes the file itself (`ISteamRemoteStorage`, no `sonic.sav` in the
  wineserver trace). `IsCloudEnabledForAccount/ForApp` now return true.
- **Idle Threads sync** (`game_specific_sync` bit `0x40`, "Idle Threads Sync"
  in the game-specific settings): loading threads poll with `Sleep()`, and
  the main thread polls them with `Sleep()` and QPC loops. Before this, each
  run did a different amount of work in the background between two frames.
  The `Sleep()` of the other threads (`NtDelayExecution`) only ends when the
  game time has advanced by the duration, and the main thread waits for the
  other threads to be idle before its `Sleep()`/`Sleep(0)` and its frame
  boundaries. "Idle" is decided from `/proc/self/task/<tid>/wchan` (ntsync,
  futex, sleep, select...); a thread that is still busy after 100 ms is
  logged at warning level (thread id, name and wait channel) and no longer
  waited for. A sleeping thread is released if the main thread is blocked on
  a wait outside a frame boundary, to avoid a deadlock. Without this option,
  even with the virtual Steam client, 2 runs out of 5 differed in the first 60
  frames.
- Tried and not the cause: audio (`audio_disabled=true` did not change the
  divergence), and the number of X events (the count in `processXlibEvents`
  varies but the rest of the trace stays identical).
- The count of non-draw frames of the first 260 frames varies between
  configurations (127 to 130 here) and is not meaningful by itself; compare
  the list of frames instead. A Lua script that logs `movie.currentFrame()`
  when `movie.isDraw() == 0`, and pauses at a fixed frame with
  `runtime.playPause()` to compare screenshots, is enough to check that runs
  stay in sync.
- Games with several threads can reach a frame boundary from different
  threads: the first stage of Sonic Generations draws from the main thread
  most of the time, and from another thread at a few frames (for example 233
  and 553 of the test movie), without changing the sequence of frames.

## Savestates

- Wine reserves tens of GB of address space, which made savestates take 15 s.
  Non-present, non-swapped anonymous pages are zero, so they are skipped
  (`82de0e07`).
- Wine duplicates the std descriptors for its handles. Pipe ends were paired by
  name, so duplicates were not registered and were closed when loading a state
  (`3553ff3c`). Half-registered pipes then crashed `FileDebug` (`6f1fa3f7`).
  Loading may still print `Recreate pipe fd was supposed to be (...) but is
  instead (...)`.
- Savefile areas in the savestate left `skip` and `uncommitted`
  uninitialized, so loading sometimes skipped their page flags and read the
  rest of the state at the wrong offsets, looping on garbage savefiles
  (`Cound not truncate the savefile  with fd 0 and size 3386706920806023168`)
  (`3994095b`). Not Wine specific, but only visible since Wine savefiles end
  up in savestates. Reaching the end of the pagemap file without its end area
  also made the loading loops keep the last area forever; a short read now
  ends the loading (`507e8721`).
- The xcb connection struct is preserved across loads using our own
  definition of it, which is smaller than libxcb 1.17's and missed the resource
  id allocator at its end. Games then reused X resource ids after loading and
  Xlib exited on `BadIDChoice` (`X_CreateGC`). We now copy the whole allocation
  (`malloc_usable_size()`) (`8a5baa09`). Not Wine specific.
- That was not enough: Xlib also caches the next resource id in its private
  `dpy->xcb->next_xid`, which the load restored to an id given to a resource
  created after the savestate. Boshy (winex11) creates GCs while running, so
  loading still ended on `BadIDChoice` (`X_CreateGC`) around frame 1306. We
  now keep the current `next_xid` across the load (`f00f281e`). Its struct
  (`Xxcbint.h`) is not installed and gained a `next_response` field between
  libX11 1.6 and 1.8, so `src/external/X11/Xxcbint.h` describes both layouts
  and the one whose `connection` and `next_xid` match the xcb connection is
  used. Reproduced natively with a GLX program creating one GC per frame.
  Not Wine specific.
- Recreating a thread that exited after the savestate uses `clone3()` with
  `set_tid`, which requires the `CAP_CHECKPOINT_RESTORE` capability. Without
  it, we fall back to `clone()`, whose x86_64 wrapper used the aarch64 syscall
  number (220, `semtimedop` on x86_64) (`af25c80a`). Not Wine specific.
- **Wine threads cannot be recreated**: they also live in wineserver, which
  forgets them when they exit, and their request pipes are closed. A recreated
  thread fails with `wine client error:<tid>: write: Bad file descriptor` and
  Wine aborts the game. For Wine games, loading a state whose threads have
  exited is refused with "Loading not allowed because a game thread has exited
  since the state was saved" (`7f7461f7`). Boshy has such a thread exiting
  around frames 1122-1192 of the imported movie. Keeping Wine threads from
  exiting while libTAS runs would lift this limitation, but is not done.
  A later Boshy freeze was this case on a build made before `7f7461f7`:
  loading at frame 1315 printed `Could not recreate thread ... with the same
  tid: Operation not permitted`, then `wine client error:19c: write: Bad file
  descriptor`, then Xlib exited on `BadIDChoice` (`X_CreateGC`). That
  `BadIDChoice` was the separate Xlib id bug above.
- **Asynchronous reads across a load** (Sonic Generations, Proton 11.0):
  loading a state in a stage sometimes ended on `free(): double free detected
  in tcache 2`, then every thread aborting (`terminate called recursively`,
  `ThreadManager.cpp:377 ... threadExit ... Bad file descriptor`). About one
  load in three or four. Wine's SIGABRT handler turns the abort into a Windows
  exception and ends the thread, so neither gdb on the core nor coredumpctl
  shows the culprit; a temporary SIGABRT handler in our `sigaction()` wrapper
  that prints `backtrace()` before chaining to Wine's showed `free()` called
  from `NtReadFile` (`ntdll.so`), on a Wine thread. Likely cause: an
  overlapped read was in flight when the state was saved. `NtReadFile`
  allocates its async request and frees it on completion, and wineserver and
  the other Wine processes (`winedevice.exe`, which serves HID devices) are
  not part of the savestate, so a read completed after the save is completed
  and freed again after the load. The reader is probably the xinput HID
  thread (`wine_xinput_hid`), which polls controllers with overlapped
  `ReadFile` on devices of `winebus.sys` in `winedevice.exe`. Testing
  `WINEDLLOVERRIDES=winebus.sys=d` (no HID devices, so no such reads);
  libtas does not give controller input to Windows games yet, so nothing is
  lost.
- **Suspend signal in 32-bit code** (WoW64): Wine runs the 32-bit code of the
  game with `%fs` loaded with a selector for its 32-bit TEB. Our suspend
  signal (`SIGXFSZ`) landing there made `stopThisThread()` crash on its first
  TLS access (`GlobalNative` → `__tls_get_addr`, SIGSEGV, "Got wrong message
  after state loading"), randomly, as only a thread caught in 32-bit code
  crashed. The handler is now `suspendSignalHandler()`, which, like Wine's
  `init_handler`, puts the `%fs` base back on the glibc thread block (found
  by tid in the thread list, as TLS and `pthread_self()` read `%fs`) when it
  differs, and reloads the selector before returning. It compares the base,
  not the selector: the 64-bit code of Wine keeps the 32-bit selector with
  the base already on the thread block, and reloading the selector there
  broke stack canaries (`*** stack smashing detected ***` after a save). The
  wrapper has no stack protector, as the canary is read through `%fs`
  (`0ed49871`).
  With this and `winebus.sys=d`, 115 saves and 62 loads in a stage of Sonic
  Generations without a crash (before: about one load in three or four).
  Not tested yet: the same without `winebus.sys=d`.

## Timing compatibility with Hourglass

libTAS inserts a non-draw frame when the game sleeps or triggers time-tracking
for longer than a frame without drawing. Hourglass only counts drawn frames, so
imported Hourglass movies desync on these frames (frames 117, 935 and 958 of
the imported Boshy movie, all from QPC time-tracking). The game-specific timing
setting "No non-draw frames (Hourglass)" (`GC_TIMING_NO_NONDRAW`) applies the
extra delay over the next drawn frames instead (`fa4764db`). It is stored in
the game config, not in the movie.

## Testing and debugging

### Display and configuration

- Run games in a dedicated rootful Xwayland, so that the compositor does not
  resize the game window (resizes break wined3d, see `Something's still
  holding back buffer`):
  `Xwayland :42 -geometry 1600x1200 -decorate -noreset`. Run libTAS there too:
  `env -u WAYLAND_DISPLAY QT_QPA_PLATFORM=xcb DISPLAY=:42 libTAS ...`
  (otherwise Qt and possibly Wine use the Wayland session).
- The compositor still sizes the rootful Xwayland window itself: a tiling
  compositor gave a restarted `:42` a portrait 1270x1403 screen, and Wine
  reports the modes of that screen, so a game whose config stores 1280x720
  refused to start. Check `xdpyinfo | grep dimensions` before blaming libTAS.
  Under niri, make the window floating and size it:
  `niri msg action toggle-window-floating --id <id>` and
  `set-window-width`/`set-window-height` (ids from `niri msg --json windows`,
  title `Xwayland on :42`). niri keeps the position of the tile, which can
  leave the floating window partly off screen: also run `center-window`.
  `docs/guides/sonic-generations.md` does it (step 1).
- Killing a Wine game with SIGKILL can leave its windows on the test display
  (same names as the next run's), so `xdotool search --name` may pick a dead
  one. Restart the test Xwayland, or check that only one window matches.
- The libTAS window size changes between runs; resize it
  (`xdotool windowsize`) before clicking its buttons.
- Settings given with `-s` are no longer saved in the ini files (`092fa5ab`), and `-s`
  accepts the Proton (`use_proton`, `proton_path`), game and directory
  (`savestatedir`, `datadir`, ...) settings. Still use a private copy of the
  config with `XDG_CONFIG_HOME`, since the UI saves other changes.
  Per-game logging settings override `-s`; edit the copy directly. `logging_status=2` logs to `<game>.log` next to the game.
  Before `08788282` that file was overwritten from its start without being
  truncated, so older logs hold several runs: the newest one is the start of
  the file, up to the first line that begins mid-sentence.
- Check that the running build has the fixes you expect: a libTAS UI started
  before a rebuild keeps its old binary, and the game loads `libtas.so` as it
  was when the game was started. Compare the build time with the commit times.
  The launcher injects the `libtas.so` next to its binary
  (`build/src/program/libtas.so`), which `cmake --build` does not update: only
  `build.sh` (through the AppDir) or a manual copy does. After a plain cmake
  build, copy `build/src/library/libtas.so` there, or pass
  `--libtas-so-path build/src/library/libtas.so`.
- Run tests from a copy of the game directory, so that save files of the user
  are not modified.
- In `-n` (non-interactive) mode the game starts unpaused and fast-forwarded:
  calling `runtime.playPause()` pauses it. Lua callbacks can drive inputs
  (`input.setKey()` in `onInput`), and `runtime.saveState()` /
  `runtime.loadState()` work, but Lua upvalues and globals behave strangely
  after a savestate, so key the actions on frame numbers only.
- Count non-draw frames by logging `LCF_SOCKET` and counting `Send socket
  message 44` (`MSGB_NONDRAW_FRAME`).
- Small Windows test programs can be built with `i686-w64-mingw32-gcc` (add
  `-lopengl32 -lgdi32` for a GL window).
- To check audio without playing on the speakers, use `PULSE_SERVER` pointing
  nowhere and `ALSA_CONFIG_PATH` with `pcm.!default { type null }`, or dump with
  `-d` and run `ffmpeg -af volumedetect` on the result.

### Stacks and faults

- `kernel.yama.ptrace_scope=1` prevents attaching to the game; set it to 0
  temporarily.
- gdb gets confused by Wine's loader (wrong architecture, no shared libraries).
  `eu-stack -s -p <pid>` gives usable unix-side stacks. Stacks stop at
  `__wine_syscall_dispatcher`.
- To get the stack of a fault that Wine turns into an exception, attach gdb
  early with `handle SIGSEGV stop nopass`, continue until the fault pc is in
  `libtas.so` (Wine uses SIGSEGV internally), then `generate-core-file` and run
  `eu-stack --core`. Attach to the process that has `winex11.so` mapped, not to
  the WoW64 launcher process.
- Useful `WINEDEBUG` channels: `+seh` (faults in unix calls), `+sync` (wait
  timeouts per thread; absolute deadlines are real time), `+dsound`,
  `+mmdevapi`, `+loaddll`.
- Xlib's default error handler prints `X Error of failed request` and exits,
  which looks like a freeze from libTAS.
- A game that stops advancing at an early frame may be showing a message box
  (Sonic Generations' configuration errors): capture the screen, or use
  `WINEDEBUG=+msgbox` (`MSGBOX_OnInit` prints the text). In fast-forward the
  frame count keeps increasing while the box is shown.
- `PROTON_LOG=1 PROTON_LOG_DIR=<dir>` writes `steam-<appid>.log` with the
  `WINEDEBUG` channels you set. libTAS logs can contain binary data: use
  `grep -a`.
- To check what Direct3D 9 reports (adapter, GUID, modes), build a small
  `i686-w64-mingw32-gcc ... -ld3d9` program calling `GetAdapterIdentifier` and
  `EnumAdapterModes` and run it under the same conditions as the game.

### Tracing waits and threads

- `WINEDEBUG=+server` (Proton keeps an existing `WINEDEBUG`) logs every
  wineserver request with the calling Windows thread id: thread creation
  (`new_thread`, `init_thread` gives the unix tid to match gdb threads),
  `suspend_thread`/`resume_thread`, `create_file`, `close_handle`,
  `terminate_thread` with the exit code. Writes to regular files and ntsync
  waits don't go through wineserver. The trace is large (hundreds of MB per
  minute), filter it with `grep -a -v` on noisy pairs.
- Sonic Generations has two threads suspending and resuming each other about
  120 times per second for its whole life; a thread found in `wait_suspend`
  is usually one of them, not a hang.
- A game frozen in `XFilterEvent()`/`_XLockDisplay()`: print
  `dpy->lock->locking_thread` in gdb (libX11 has debug info through
  debuginfod) and look for that pthread in `thread apply all bt`. This found
  the OSD event queue deadlock with `wine_dinput_wor`, which calls `XFlush()`
  with the display locked (`4332d336`).
- winedbg crashes when attaching to a game under libtas.
- `-n` turns fast-forward on; add `-s fastforward=false` to keep the real
  speed (`e7c21d5f`).
- The virtual Steam client of Windows games logs to `<datadir>/steam_api.log`
  with the log level of the game (Steam category) (`2f840ced`): use level
  Debug or Trace to see which Steam calls a game makes.

## Open problems

- The Windows steam_api.dll replacement has no access to libtas inputs, so
  Steam controllers stay neutral in Windows games.
- SRB2 video dumps are black after the first frame, while the game renders
  correctly. Not reproduced since the encoder follows the screen capture
  (v2.2.15 under Wine, 1280x800, and a native build with `-software`: colors
  and orientation right, only the intro fades are dark; each is split in two
  files at the first window resize, from 320x200 to the real size, as
  before).
- Directory listings, renames and directory creation for savefiles (see Files).
- Savestates cannot be loaded across the exit of a Wine thread.
- After loading a state, the stdout of the test program was not output anymore
  (probably related to pipe recreation).
- Clickteam games may also enumerate other directories for their own files.
- Without a custom screen resolution, the display modes that Wine reports come
  from the real X display (Wine collects them in its `explorer.exe` process).
  Games that check their saved resolution (Sonic Generations) fail on a
  smaller or rotated display, and the modes are not deterministic across
  machines. With a custom resolution, `explorer.exe` gets the fake XRandR
  screen too (see [Proton](#proton)), not yet verified for other games.
- Other Wine processes (wineserver, `winedevice.exe`, `explorer.exe`) are not
  savestated, so any game state they hold (pending async I/O, HID devices of
  `winebus.sys`, xinput polling) can disagree with the game after a load.
  See "Asynchronous reads across a load" in Savestates.
- Idle Threads sync is only checked on Sonic Generations, with the virtual
  Steam client: a rare fork remains in the first ~30 frames (about 1 run in
  10-20, at the boundary between two frames in the loading of the title).
  Runs of the first stage match in game time and position at frame 1500
  (6 of 6), but not pixel for pixel (about 0.1% of the pixels differ).
- Proton launch (`dc47524b`) is only tested with Proton 11.0 and Sonic
  Generations. The seccomp SIGSYS forwarding of that commit is untested.
- Loading a savestate under openbox once did nothing (1 run out of 4, not
  reproduced).
- Sonic Generations: not tested in a stage yet (loading and time-tracking,
  non-draw frames during gameplay), controllers (XInput) not tested.
