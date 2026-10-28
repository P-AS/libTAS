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
- `WARN: Unsupported stream direction 1` (capture streams) is harmless.

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
- libTAS saves the game config on exit, so `-s` overrides and test paths end up
  in the user's `~/.config/libTAS/<game>.ini`. Use a private copy with
  `XDG_CONFIG_HOME`, and point `savestatedir`/`datadir` of the copied
  `libTAS.ini` elsewhere. Per-game logging settings override `-s`; edit the
  copy directly. `logging_status=2` logs to `<game>.log` next to the game.
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

## Open problems

- The Windows steam_api.dll replacement has no access to libtas inputs, so
  Steam controllers stay neutral in Windows games.
- SRB2 video dumps are black after the first frame, while the game renders
  correctly.
- Directory listings, renames and directory creation for savefiles (see Files).
- Savestates cannot be loaded across the exit of a Wine thread.
- After loading a state, the stdout of the test program was not output anymore
  (probably related to pipe recreation).
- Clickteam games may also enumerate other directories for their own files.
