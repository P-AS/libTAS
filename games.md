# Running native Linux games

This document records what was learned while testing native Linux games under
libTAS: problems found, how they were fixed, design choices, and what is still
open. It is the equivalent of `wine.md` for games that are not Windows
programs. Keep it up to date when working on anything native-game related
(see `AGENTS.md`). Wine specific problems stay in `wine.md`.

Tested setup: Arch Linux, Mesa llvmpipe (`opengl_soft=true`), rootful Xwayland
display, libTAS built from `next` with `cmake`. Games used for testing:

- **La-Mulana** (Steam app 230700, Nigoro, version 1.5.5.2, 32-bit ELF, its
  own SDL2, OpenAL and `libsteam_api.so` in `lib/`, OpenGL 2). Works with
  `sleep_handling=3` and `virtual_steam=true`, see [La-Mulana](#la-mulana).

## Basics that matter for a native game

- A 32-bit game needs `libtas32.so`, which is not built by default
  (`LIBTAS_BUILD_I386`, or `LIBTAS_I386_LIB_ONLY` for only the 32-bit
  library). It needs the multilib toolchain (`gcc -m32`) and the 32-bit
  libraries (`lib32-glibc`, `lib32-gcc-libs`, X11, X11-xcb, asound, here from
  the Arch `lib32-*` packages). Build it in its own directory so that the
  normal build tree is left alone, and give it to the program with
  `--libtas32-so-path`:
  `cmake -S . -B /somewhere/build32 -DLIBTAS_I386_LIB_ONLY=ON -DCMAKE_CXX_FLAGS="-O2 -g" -DCMAKE_C_FLAGS="-O2 -g"`.
  Without `-g` nothing in a crash inside `libtas32.so` can be resolved.
- Audio mixing and encoding need a 32-bit `libswresample` at runtime: without
  it, the log says `Could not link to swr_alloc, disable audio mixing`, and the
  game runs silently. The `lib32-ffmpeg` package of the AUR is not used here (it
  pulls many dependencies, some orphaned, which is a security risk). The libraries
  of Ubuntu's i386 packages, from a distrobox, work on the host without installing
  anything: only four files are not already provided by the Arch `lib32-*`
  packages, and libTAS adds `<datadir>/lib_i386` (`~/.local/share/libTAS/lib_i386`)
  to the library path of a 32-bit game. libswresample is loaded with `dlopen()`,
  from `.so.2` to `.so.6`, so there is no need to rebuild:
  `distrobox enter ubuntu -- sh -c 'cd /usr/lib/i386-linux-gnu && cp -L
  libswresample.so.6 libavutil.so.60 libsoxr.so.0 libOpenCL.so.1
  ~/.local/share/libTAS/lib_i386/'` (with `libswresample6:i386` in the box,
  Ubuntu 26.04). These were tested with `libtas32.so` built with the headers of
  FFmpeg 9 of Arch and the libraries of FFmpeg 8 of Ubuntu.
- libTAS has one "main thread": the thread that last reached a frame boundary
  (a buffer swap, or enough accumulated sleep). The frame count, the
  deterministic time and the audio mixing advance only at its boundaries.
  Which threads advance game time with their sleeps is the `sleep_handling`
  setting (`0` never, `1` only the main thread, `2` every thread, `3` only the
  main thread while the other threads wait for the game time). The
  settings of the debug pane are `debug_state` (bits: `1` uncontrolled time,
  `2` native events, `4` keep main thread as first thread, `8` native file
  I/O, `16` native internet); it can be set with `-s debug_state=N` (added
  for this document, it was only in the UI).
- With `debug_state` bit `1` (uncontrolled time) the game sees the real
  clock: nothing is deterministic, so it is only useful to find out whether a
  game works at all, but the frame counter then counts the frames that the
  game draws (the title shows the real fps).
- `-s` settings are not saved in the ini files, and the settings of a per-game
  ini win over `-s` only for the logging keys that the ini holds.

## La-Mulana

The game works with controlled time: use `sleep_handling=3` ("Main thread
advances time, other threads wait" in the runtime settings) and
`virtual_steam=true`. Inputs, movies and savestates work, and runs are
deterministic (see [Results](#results-with-sleep_handling3)). It needs
`libtas32.so`.

The TASVideos compatibility page says: "Only run with Uncontrolled time and
Native events. At least three different threads render on screen". Findings:

### How the game is built

Found with `eu-stack -p <pid>` (works on a 32-bit process, the binary has
symbols) and `nm`/`objdump -d -M intel` on `LaMulana.bin.x86`:

- **First thread** (`main`): `SteamAPI_Init`, SDL init, `freopen64` of stdout
  and stderr into `log-out.txt` and `log-err.txt` in the preferences folder,
  creates the window and the first GL context, starts the other threads, and
  then only runs `loopIteration()`: `SDL_PollEvent()` in a loop with no sleep
  (5 million calls in about 14 s under libTAS), window and resolution requests. It
  never draws and never sleeps. It makes the GL context current a few times
  at startup.
- **Logic thread** (`lamulanaMain` then `lamulanaMainThreadIteration()`): runs
  the game as a state machine on a global (`0x89688c0` in the 1.5.5.2 binary).
  Startup states: 2 (`LamulnaInitialize`), 3 (loading), then 4 (loading
  finished, the game proper; names are guesses from the function names). It
  takes a mutex and calls `glXMakeCurrent()` twice per cycle, to load and
  draw with the single GL context (74 calls in the stuck controlled run, 1100
  in the 766 frames of the working run), and waits with `OSSleepTicks()`
  (`SDL_Delay()`) between iterations. In state 3 it only calls
  `OSSleepTicks(500)` over and over.
- **System thread** (`lamulanaSystemThread`, `mySleep()`): runs callbacks that
  `lamulanaMain` registers with `setSystemCallBack()`: `checktimer` every
  1000 ticks, and `initDraw`, which draws the loading screen
  (`drawNowLoding()`) and, when loading is done, switches the game to state 4
  with `changeLamulanaThread(4)`. So the loading screen is drawn by this
  thread, not by the first one.
- **Music thread** (`musicThread`): `SDL_Delay()` loop.

That is the "three threads that render": the first thread creates the context,
the system thread draws the loading screen, and the logic thread draws the
game, all with `glXMakeCurrent()` hand-offs of one context.

### What happens under libTAS

Each configuration was run for 25 to 40 s at normal speed, with the
virtual Steam client (see below) and the state of the game read from its
memory: `/proc/<pid>/mem` at the address of the state global (`0x89688c0`,
and `0x89688c4`, set to 2 by `initDraw` when it draws). A small python script
that opens the file, seeks and unpacks a 32-bit word is enough, and it works
because the ptrace scope is 0.

| Configuration | Result |
|---|---|
| default (`sleep_handling=1`) | stuck at frame 0, state 3, `initDraw` never draws |
| `sleep_handling=2` | frames advance (1400 frames in 26 s, the system thread triggers them with its sleeps), 3 of them draws, then a still picture with a few blue marks (checked over 40 s); state 3, `initDraw` never runs (the system thread is starved, see below) |
| `sleep_handling=2` with Idle Threads sync (`game_specific_sync=64`), or native events (`debug_state=2`), or `async_events=3` | same as above |
| `sleep_handling=0` | `initDraw` runs (`0x89688c4 = 2`), but the frame counter stays at 1 (checked 8 s apart): stuck in state 3 |
| `sleep_handling=3` | works, deterministic, see below: state 4, the intro, the menu and the village at frame 1500, 60 fps |
| `debug_state=1` (uncontrolled time), with or without native events | works: state 4, the intro text ("It is where I belong. Children, please help"), 150 to 250 fps, frame counter counts draws |

Reasons, from the traces:

- With the default `sleep_handling=1` only sleeps of the main thread advance
  time, and the main thread is the first thread (it never sleeps) until some
  thread reaches a frame boundary. The game threads wait for `SDL_GetTicks()`
  to move, and it does not.
- With `sleep_handling=2` the system thread (`mySleep()`) becomes the main
  thread and makes frames, but `initDraw` never runs. The loop of that thread
  calls `lamulanaSystemThreadIteration()` (the timer callback when 1000 ms
  have passed, then `initDraw` every time) and `mySleep(3)`. Reading its
  variables from the process: `initDraw` is still registered, its marker
  (`0x89688c4`) is 0 and the thread's own timer (`0x89687ac`) is stuck at
  1016 ms, so it never finishes an iteration. The cause is in
  `DeterministicTimer::addDelay()`: it adds the sleep to the game time and to
  a debt that only the main thread pays, by making frame boundaries (60 per
  real second, so at most 1 s of game time per real second). In this mode
  every sleeping thread adds its duration (the logic thread sleeps 500 ms in a
  loop, the music thread 30 ms, the system thread 3 ms), about 2 to 3 s of
  debt per real second. The main thread, which here is the system thread, stays
  in the `while (addedDelay > maxDeferredDelay) frameBoundary()` loop forever
  (7 stack samples out of 8 in `exitFrameBoundary`) and never runs its own loop.
- The flags of the game are one word (`0x89687dc`, `dsystemFlag(mask, 0/1/2)`
  tests, sets and clears): `0x10` (set at the end of the init of `lamulanaMain`)
  in the stuck runs, `0x14` in the working ones; flag 1 is only set by the
  reset code and flag 8 by save and error code, so they were not the cause.
- The trace of the working (uncontrolled) run shows the two drawing threads,
  with `SDL_GetTicks()` called 56 000 times in 766 frames by the thread that
  draws (a spin on the clock), and 1100 `glXMakeCurrent()` pairs.

### The `sleep_handling=3` mode

Added in `55b694ae`. Design, in `DeterministicTimer`, `transfer_sleep()` and `SDL_PollEvent()`:

- A sleep of a secondary thread calls `waitDeterministicDelay()`: it ends when
  the game time has advanced by its duration, and does not add to the delay.
  The debt that made `sleep_handling=2` starve the system thread is gone.
- 16 empty `SDL_PollEvent()` calls in one frame (`countEmptyPoll()`) count as a
  1 ms sleep. The first thread is a pure event pump that spins on it, and it is
  the only thread that can drive the clock before the loading screen draws.
  Once another thread is main, the pump waits for the next frame instead of
  spinning. The threshold is per frame, so that a game that drains its queue
  once per iteration is not affected. Busy loop detection only counts the time
  functions, so it does not see this loop.
- The main thread waits for the other threads to be idle before and after each
  of its sleeps (`addDelaySynced()`) and each frame boundary, for up to 5 s
  (100 ms for the Idle Threads sync of Wine, which is not enough for loadings).
  This is what makes the number of frames of a loading independent of the speed
  of the machine. Before it, a run under CPU load had 84 non-draw frames instead
  of 89 and another RNG seed (`first_seed` is taken from the time).
- A sleeping thread is registered with its deadline (`waiting_deadlines`), so
  that the main thread also waits for a thread that should wake up but has not
  been scheduled yet, and it wakes up after a frame boundary, not during it, so
  that a savestate does not catch it in the middle of its work.
- A sleep is ended early if the main thread stays blocked on a wait for about
  100 ms, to avoid a deadlock when it waits for that thread. It takes that long
  because the main thread regularly blocks for an instant on a lock or on the
  driver, which would end sleeps at a point that depends on the machine.
- `is_exiting` turns all of this off: time does not advance anymore, so waiting
  threads do real sleeps and the main thread no longer waits for them. Without
  it, the sleeping loops become busy loops and the game takes 5 s per sleep to
  exit, and the movie is not saved in time.

This is a separate mode, and only the code paths above check for it, so that
the Wine Idle Threads sync behaves as before. The mode implies the waits of
that sync, so `game_specific_sync=64` is not needed.

### Results with `sleep_handling=3`

Inputs are read on the logic thread with `SDL_GetKeyboardState()` and the SDL
controller API (`getKeyb()`, `getCont()`), not from the events of the pump
thread, so they do not race with it. Tested with keys from a Lua script
(Return, `z` and space to skip the intro and the menu, then arrows to walk and
jump), which reach the village around frame 1400. Comparison at frame 1500 of
the screenshot and of a dump of `.data` and `.bss` (1.48M words), ignoring words
that are pointers in both runs (allocations differ):

| Compared with a live run | Non-pointer differences | Picture |
|---|---|---|
| Another live run, and one with 16 busy processes at the same time | 0 | identical |
| A run that saves at frame 1400 and loads at 1450 (0.12 s and 0.16 s) | 0 | identical except the FPS of the OSD |
| Playback of the recorded movie, without any live input | 0 | identical except the OSD (`1500/1510`) |

These runs were at normal speed, without fast-forward, on llvmpipe.

Fast-forward (`fastforward=true`) does not change the game state. Frame 1500 has
no non-pointer difference with the run at normal speed, in about 6 s instead of
27 s, and the picture is identical (with `fastforward_render=0`) except for the
FPS of the OSD and the pointer. Longer runs, with random inputs from frame 1500
(walks, jumps, attacks), leave the village for a room with snakes and pick up
coins, so that they use the RNG. All of them are compared with a fast-forwarded
live run, with no non-pointer difference:

| Compared with a live run | Checked at frame |
|---|---|
| Another live run | 9000 |
| A run that saves at frame 7000 and loads at 7500 | 8000 |
| Playback of the movie recorded at that speed | 8000 |

Audio mixing (see [Basics](#basics-that-matter-for-a-native-game)) was compared
with encodes, whose decoded audio and raw video were hashed. `audio_mute=true`
with an encode still mixes, because encoding forces it, without playing anything
on the speakers. Over 1510 frames with inputs, two runs at normal speed and one
fast-forwarded have the same audio and the same video on all frames. So does the
audio of a recorded movie and of its playback. Mixing does not change the game
state (no non-pointer difference at frame 1500 with a run without mixing). Over
3800 fast-forwarded frames with the random inputs, two runs have the same audio
and video. The audio is not silent (-15 dB on average).

A run with 16 busy processes reached frame 7786 before the 30 s limit, and its
trace is identical to the live run up to there (its memory was not compared).
No run went past frame 9000, and past frame 1510 nothing was compared with a run
at normal speed, which would take about 150 s.

### Making a movie

- A movie stores the framerate, the initial times, the time-tracking, the
  controllers and the mouse, which are enforced when it is loaded. It also stores
  `sleep_handling`, which is not enforced: libTAS warns when playing the movie
  with another value (a dialog, and on stderr, as `-n` shows no dialog), except
  with "Don't enforce movie settings" or for older movies that don't have it.
  `virtual_steam` is not stored, so it must be given with the movie.
- All tests started without any file of the game, in a private `XDG_DATA_HOME`.
  The game keeps `config.ini` and `save/` in `$XDG_DATA_HOME/nigoro/la-mulana`,
  and reads what is there: "Prevent writing to disk" stops writes, not reads.
  A movie made with existing files is not expected to sync without the same
  files (not tested). `XDG_DATA_HOME` also moves the data directory of libTAS,
  including `lib_i386`, if set for libTAS and not only for the game.
- Avoid inputs on frames 280 to 283, see Open problems.

### Logs were lost: the game reopens stderr

libTAS wrote its log with `fputs(..., stderr)`. The game does
`freopen64("log-err.txt")` on stderr in `main()`, so after the first 700 lines
(the ones before the `freopen`) the log went into the game's file, which stays
in memory with "Prevent writing to disk": the log looked frozen at frame 0 and
the runs looked stuck while frames were advancing (the libTAS window showed
frame 1400 with a log that ended at frame 0). The library now keeps its own
copy of the original stderr (`F_DUPFD_CLOEXEC` in a constructor of
`logging.cpp`) and writes the log there. This affects any game that
reopens or closes its stderr. In the traces, log lines carry the thread id
(`t:<tid>`, with an `M` suffix for the main thread), which is how the threads
above were told apart.

The same logging change had a deadlock: to know if the log goes to a terminal,
`debuglogfull()` called the hooked `fileno()`, which takes the lock of the
savefile list, and `SaveFileList::openSaveFile()` already holds it while
`SaveFile::open()` logs. Any game that opened a savefile with `INFO` logs enabled
froze; La-Mulana stayed on a black screen in `createNandArea()` while frames kept
advancing (the stack is the giveaway, `SaveFile::open` then `debuglogfull` then
`fileno`). `GlobalNoLog` only disables logging, not the hooks, so it needs
`NATIVECALL` (fixed in `55b694ae`).

### Steam

`main()` calls `SteamAPI_Init()` and shows a message box when it fails
("Steam must be running to play this game" was reported by the user). Use
`virtual_steam=true` to avoid needing the client. The 32-bit
`libsteam_api.so` of the game is what the virtual client replaces.

### Not done / not checked

- Only a few rooms near the village were played (9000 frames at most), with
  random inputs: no boss, no save with the in-game tablet, no reload of a save.
- A controller: only the keyboard was tested.
- Playing the audio on a real device (`audio_mute=false` was not used, to keep
  the tests silent). It only sends the mixed samples to ALSA.
- A real GPU (`opengl_soft=false`).
- Whether the native events setting matters for input in the uncontrolled run
  (the page says it is needed; the game runs without it).

## sdl2-compat and sdl12-compat

Fixed in 4cc32a10 for sdl2-compat.

`sdl2-compat` provides `libSDL2-2.0.so.0` and implements it on top of `libSDL3`.
Its constructor loads SDL3 with `dlopen(RTLD_LOCAL)` and all its functions with
`dlsym(handle, "SDL_...")`. libTAS expected one version of SDL per game, so a game
crashed at startup (`X_CreateWindow BadValue`, a window of size 0):

- Our `dlsym()` returned our hooks for these names, so that the SDL3 functions of
  sdl2-compat came back into libTAS.
- SDL3 reads `SDL3_DYNAMIC_API` and calls our `SDL_DYNAPI_entry()` with API version
  2, which overwrote the table of original functions, and the game was
  treated as an SDL3 game: SDL2 arguments of `SDL_CreateWindow()` were read as SDL3
  ones.

Design choice: such a game is an SDL2 game, sdl2-compat is its SDL2 library, and
SDL3 stays unhooked, like the internals of the regular SDL2 library. What is below
SDL3 (X11, OpenGL, time) is still hooked. `dlsym()` returns the real function when
`libSDL2-2.0` looks up an `SDL_` symbol that resolves to `libSDL3`, which also
detects sdl2-compat (`isSDL2Compat()`). `SDL_DYNAPI_entry()` then fills the table of
SDL3 with its own functions. A real SDL3 game is not affected.

`WAYLAND_DISPLAY` does not matter, even if SDL3 prefers Wayland, as libTAS already
clears it for the game.

SDL3 calls `nanosleep(10ms)` inside `SDL_CreateWindow()` (2 calls) and
`SDL_CreateRenderer()` (3 calls), where the regular SDL2 library doesn't sleep.
These added non-draw frames, which shifted the inputs and desynced a movie recorded
with the regular SDL2 library. A sleep called from `libSDL3` now only sleeps, without
adding to the game time. In case SDL3 is waiting for a timeout, sleeps of the main
thread count again after 100 calls in the same frame, to prevent a softlock.

Checked with sdl2-compat 2.32.74 and SDL 3.4.20: `simplestgame` and
`lsdl2srb2legacy` with the OSD and inputs, the same picture at frame 450 over two
runs, and with a savestate (save at 300, load at 400). A 2777-frame movie of
`lsdl2srb2legacy` recorded with the regular SDL2 has no differing pixel at frame 900
between both libraries; the rest of the movie was not compared.

32-bit, with lib32-sdl2-compat 2.32.74 and lib32-sdl3 3.4.18: `simplestgame` built
with `-m32` has inputs, the OSD, the same picture over two runs and with a savestate,
and only the OSD differs from the regular 32-bit SDL2 at frame 200. La-Mulana and
Shovel Knight are not concerned: they bundle their own SDL2 through their `RPATH`.

`sdl12-compat` provides `libSDL-1.2.so.0` on top of a private `libSDL2`, which can
itself be sdl2-compat, so that a game can load three SDL libraries. It uses the same
design: the game is an SDL1 game, `dlsym()` returns the real function when
`libSDL-1.2` looks up an `SDL_` symbol in `libSDL2-2.0` (`isSDL12Compat()`), and
`SDL_DYNAPI_entry()` doesn't hook this SDL2. Without this, the game aborted in
`ImGui::NewFrame()`: sdl12-compat presents with OpenGL inside `SDL_SetVideoMode()`,
and this swap ran a frame boundary with `RenderHUD_GL`, which created the ImGui
context. The next `SDL_Flip()` then used the base `RenderHUD`, which starts ImGui
frames without ever rendering them. Now `glXSwapBuffers()` and `eglSwapBuffers()`
called from these private libraries (`isSDLCompatInternal()`) only swap, and their
sleeps don't count either.

libTAS sets `SDL12COMPAT_OPENGL_SCALING=0`: otherwise the OpenGL of the game goes
to a framebuffer of sdl12-compat, copied to the window at the swap, over our OSD.
Nothing draws the OSD for an SDL1 game using surfaces (`SDL_Flip()`), as with the
regular SDL 1.2.

Checked with sdl12-compat 1.2.68 on sdl2-compat, on `simplestsdl12game` (inputs, same
picture over two runs and with a savestate) and an OpenGL program (OSD). No
comparison with the regular SDL 1.2 library, which is not available here, so
movies recorded with it are not checked. There is no 32-bit package of sdl12-compat
in the repositories, so 32-bit is not tested.

To compare with the regular SDL2 library without uninstalling sdl2-compat, extract an
`sdl2` package from `/var/cache/pacman/pkg` and export `LD_LIBRARY_PATH` to its
`usr/lib` when starting libTAS. Check `/proc/<pid>/maps` of the game to be sure.

## Regression check of the changes for La-Mulana

Old build (the commit before) against new build, two runs each at normal speed
with the default sleep handling, comparing the draw/non-draw list and a screenshot
at a fixed frame:

| Program | Frame | Result |
|---|---|---|
| `glxgears` | 300 | same frames, picture differs only in the FPS of the OSD (also between two old runs) |
| `simplestgame` (no window) | 300 | same frames |
| `lsdl2srb2legacy` (menu inputs, `-opengl`) | 640 | same frames, no differing pixel in the whole display |
| Boshy (Wine, private prefix, menu inputs) | 800 | same frames, no differing pixel, with and without Idle Threads sync |

The old build needed `logging_level=2` for these comparisons, because of the
deadlock on savefiles above: at the level 3, it froze `lsdl2srb2legacy` at frame
587 (`G_SaveGame()`), and Boshy never reached its first frame. Also, a savestate
of `lsdl2srb2legacy` (save at 520, load at 570) gives the same picture at frame
640. Sonic Generations, the main user of Idle Threads sync, was not run.

## Pointer warp on Xwayland

"Warp mouse to center" (`mouse_warp`, with relative mouse mode) reads the pointer
and warps it to the center of the game window on each frame, from the libTAS
program. This is independent of the game, so it also concerns Windows games.

- `xcb_warp_pointer()` was only buffered, then sent with the next request: the
  `haveFocus()` of the next frame, just before the pointer is read. So the game
  always got a motion of 0, on any X server. `warpPointer()` now flushes.
- Xwayland only honors a warp while the cursor is hidden: it locks the Wayland
  pointer with `zwp_locked_pointer_v1` and feeds relative motion to the X pointer
  (`xwl_seat_emulate_pointer_warp()` returns if `x_cursor` is set). With a visible
  cursor, the warp only changes the position of the X pointer until the next real
  motion, so that the position read after it is fake and the deltas are wrong. A
  game can't hide the cursor, as we noop `XDefineCursor()`.
- So on Xwayland (`XWAYLAND` extension present, nothing changes on a real X server),
  `KeyMappingXcb::capturePointer()` grabs the pointer with a blank cursor and
  `confine_to` the game window. Warps then work for any client, not only the owner
  of the window. Design choices:
  - The cursor is an argument of the grab, instead of `XDefineCursor()` on the game
    window, so the server restores everything when the grab ends, including if
    libTAS is killed (checked with SIGKILL). An empty cursor counts as hidden:
    `mi` turns it into `NullCursor`. `XFixesHideCursor()` also works but hides
    the cursor on all the windows of the screen, including the libTAS UI.
  - The confinement is needed: without it, a motion that goes past the window edge
    in a single frame destroys the warp emulator and the pointer escapes for good.
  - `GameLoop::updatePointerCapture()` holds the pointer only when the game is
    running, has the focus and inputs are not read from a movie. Otherwise the
    libTAS UI could not be reached.
  - A frame advance while paused does a short grab, warp and ungrab: Xwayland gives
    the warped position as `set_cursor_position_hint`, used when the lock ends, so
    the real pointer moves to the center. The hint is optional for the compositor.
- Checked with `simplestgame` on niri + xwayland-satellite, rootful Xwayland on
  niri, and Xvfb: the sum of the deltas seen by Lua matches the motion sent
  (Xvfb only checks the flush, as it does not grab). Not checked on GNOME, KDE,
  a physical mouse, or a Windows game.
- A single motion larger than the window is not limited to the window: it was only
  limited by the screen size (959 for a center at 320 on a 1280 wide screen), and
  the pointer stays held. Why the confinement does not limit it here, while it does
  in a small Xlib test, was not investigated.

## Pointer position outside of X11 windows

Rootless Xwayland only knows the position of the pointer on its windows. Elsewhere,
`XQueryPointer()` keeps returning the last position and success. When the pointer is
over another X window, it even returns coordinates relative to that window, as
xwayland-satellite places all windows at (0,0) of the root. A game then gets wrong
positions, instead of the ones outside of its window that it gets on a real X server.

There is no common Wayland way to get the position, but most compositors have one.
`src/program/pointer/` has an implementation of `GlobalPointer` for each, selected by
`mouse_outside_source` (0 disabled, 1 automatic, 2 image capture, 3 KWin, 4 Hyprland,
5 screencast), and only used on Xwayland (`XWAYLAND` extension):

| Compositor | Way | Tested with |
|---|---|---|
| niri >= v26.04-276, cosmic-comp, sway and wlroots, Wayfire, Hyprland | `ext_image_copy_capture` cursor session on each output (`SOURCE_IMAGE_CAPTURE`) | libTAS on niri-git and cosmic-comp, own client on a real niri-git session; sources only for the others |
| KWin 6.8 | Script loaded over D-Bus, calling us on `workspace.cursorPosChanged` | libTAS |
| Hyprland | `/cursorpos` on `.socket.sock`, in a new connection each time | Fake socket only; sources |
| Mutter 51, gnome-shell, niri 26.04 | `org.gnome.Mutter.ScreenCast` with `cursor-mode=2`, read from `SPA_META_Cursor` | libTAS on Mutter; own client on gnome-shell, on a nested niri-git and on a real niri-git session |

Design choices:

- The screencast session is stopped with `Session.Stop` when we are done. Mutter ends
  it when our D-Bus connection closes, but niri keeps it, listed as inactive in
  `niri msg casts`.
- The screencast is never picked automatically: it is a real screencast, with a cost,
  and users may be notified. A shell extension is not used on GNOME, as it needs a new
  login to be loaded. `org.gnome.Shell.Eval` returns `(false, '')` outside of
  `--unsafe-mode`.
- The screencast only offers formats without a modifier, to get shared memory buffers.
  We don't look at the images, and with modifiers niri 26.04 stopped the cast (`wrong
  modifier choice type`): when a single modifier is common with us, it gets one value
  instead of the list that it expects. This only shows on a real session.
- A cursor session on a window source is useless: the protocol sends no position for a
  cursor outside of the source. So we use the outputs.
- No backend gives the position of the window. `PointerCalibration` compares the
  position from the compositor with the one from Xwayland while the pointer is on the
  window, and measures a scale and an offset for each axis. So nothing depends on how a
  compositor exposes windows, and Xwayland scaling should be handled.
- We can't rely on the output transform: on the same flipped winit output, niri reports
  the top-left as (64,40) and cosmic-comp as (64,760). Measuring a negative scale
  handles this, but not a rotation by 90 degrees.
- The child of the root in `xcb_query_pointer()` tells if the pointer is on the game:
  it is the top-level of the window under the pointer, or 0 when it is on no X window
  (Xwayland resets the sprite trace in `sprite_check_lost_focus()`). This is not enough:
- Xwayland can keep saying that the pointer is on the window with a stale position, so
  a sample is only used if Xwayland moved when the compositor did (seen on rootful
  Xwayland). Before any motion it also gives a meaningless position that it says is on
  the game, so samples are only used after seeing it move. Otherwise, one bad sample
  breaks all positions outside of the window.
- Staleness is "on some axis, the compositor moved and Xwayland did not, since they
  last agreed, for 3 reads". Only whether each axis moved is compared, as the scale
  and the direction of an axis may not be known yet. A first version compared with
  the compositor position at the last Xwayland motion, which broke on Mutter: the
  screencast is a frame behind Xwayland, so its catch-up looked like a lost pointer
  and every later position was stale.
- The first sample may be taken while the pointer moves, and then pair positions of
  different times (a 3 px offset on Mutter). It is kept until the first sample taken
  at rest, and the check that rejects samples where only the compositor moved only
  compares with samples taken at rest, or that bad sample would block all others.
- Queries never block for long. Wayland and D-Bus events are read without waiting, and
  Hyprland is given 10 ms before reusing the last position.
- `mouse_warp` does not use this: while we hold the pointer, it can't leave.

Limits: a window moved while the pointer is outside keeps the old offset until the
pointer comes back. A scale needs two measurements of at least 64 pixels, taken while
the pointer rests on the window for 150 ms, and until then it is assumed to be 1. Clicks outside
the window still go to what is under the pointer. Hyprland only gives the cursor session
if `ecosystem:enforce_permissions` allows it, but its socket does not need this.

Testing: a niri nested in headless weston only offers the Mutter D-Bus interfaces with
`debug { dbus-interfaces-in-non-session-instances; }` in its config, run inside its own
`dbus-run-session`. With shared memory buffers, its screencast then works even without
a GBM device.

Not done:

- Several monitors are only implemented, not checked: the session of each output and
  `zxdg_output` for the image capture, `GetCurrentState` for the screencast. Rotated
  outputs are not handled.
- Weston: `pointer_position` is only in its test plugin (`weston-test.so`), which is
  not installed. It would need a plugin of ours passed with `--modules`, so that rootful
  Xwayland is the answer there.
- WSLg, an idea not tried, as it needs a Windows machine. WSLg runs a Weston with the
  RDP backend, and `WSLGd` hardcodes its arguments (`--backend=rdp-backend.so
  --modules=wslgd-notify.so --xwayland`), so no plugin. Pointer input only comes from
  mstsc over RDP while the pointer is on one of its windows, so nothing on the Linux
  side can know the position. But the Windows host knows it, and WSL can start Windows
  programs (`WSL_INTEROP` being set is also how to detect it):
  - Build a small `libtas-cursorpos.exe` with mingw, as we already do for the xinput
    dlls, that prints `GetCursorPos()` when asked on stdin, and start it once so that
    each query is only a pipe round trip. PowerShell would work to prototype, but takes
    about a second to start.
  - This gives Windows virtual screen coordinates. `PointerCalibration` should cope with
    WSLg scaling as with any compositor, which is the first thing to check, along with
    `GetPhysicalCursorPos()` on displays with different DPI, and whether window moves on
    the host are seen as often as the calibration needs.
  - The helper has to live somewhere Windows can execute, and interop can be disabled
    (`[interop] enabled=false` in `wsl.conf`), so it must fail quietly.

## Testing and debugging

- To test `mouse_outside_source`, run libTAS in absolute mouse mode and log
  `input.getMouseCoords()` from Lua while moving the pointer. A compositor can be nested in
  the private niri (`cosmic-comp`), or use its own input: `kwin_wayland --virtual
  --xwayland` with `org_kde_kwin_fake_input` and `KWIN_WAYLAND_NO_PERMISSION_CHECKS=1`,
  `mutter --headless` with `org.gnome.Mutter.RemoteDesktop` (its Xwayland needs
  `XAUTHORITY=$XDG_RUNTIME_DIR/.mutter-Xwaylandauth.*`). Use `dbus-run-session`.
  `kwin_wayland --wayland-display` crashes on the headless weston, and niri can't start a
  screencast when nested (`no GBM device available`). With a fake input device, Qt 6 crashes
  in `xi2HandleDeviceChangedEvent()`: set `QT_XCB_NO_XI2=1`.
- Pointer behavior can be tested on the real stack without touching the pointer of
  the session or showing a window: `weston --backend=headless --renderer=gl
  --socket=X`, a private `niri -c cfg -- cmd` with `WAYLAND_DISPLAY=X` and without
  `NIRI_SOCKET`, and a client moving the pointer with `zwlr_virtual_pointer_v1`.
  Nesting niri in the session on a hidden workspace is too slow, as it gets no frame
  callbacks. An `XGrabPointer()` from another client tells if the pointer is held
  (`AlreadyGrabbed`). Traps: a server where no client has defined a cursor has it
  hidden (`CursorVisible`), so warps seem to work; absolute virtual pointer motion
  escapes a lock on niri, unlike a mouse; the first `xdotool key` was lost under
  xwayland-satellite (not on rootful Xwayland), so send a throwaway key first;
  `xdotool search` finds nothing under xwayland-satellite, use `xwininfo -root -tree`.
- A crash at exit of `lsdl2srb2legacy` was on SIGTERM only (10 runs out of 15): its
  handler runs `I_Quit()`. The signal interrupted the real sleep of
  `exitFrameBoundary()`, which is a native section, so the whole shutdown ran in
  native state, and the Mesa llvmpipe threads were joined with the real
  `pthread_join()`, but all our threads are detached by `pthread_create()`. This
  read a freed stack, or corrupted the heap (`unlink_chunk` at the exit of a thread).
  The join functions now only call the real function for threads that we don't
  manage. Stopping a game from libTAS uses SIGKILL and `timeout` sends SIGTERM to its
  process group, so it showed up only with test scripts. To test a shutdown, send
  SIGTERM to the game alone: with `timeout`, libTAS also kills the game, which hides
  a hang. The handler can still interrupt Mesa in a GL call: one run before this fix
  hung in `delete_textures()`. This is a limit of shutting down from a signal handler.
- `kill -9` while a core is written truncates it. There are no stacks, but the first
  thread of the notes is the faulting one: its registers and signal address are in
  the file, and the `NT_FILE` note maps them to a library. Let the game die by itself
  to get a full core.
- SDL2 dynapi indices (`index_sdl2`) must match the jump table of the real
  library. `SDL2_dynapi_procs.h` has `__LINUX__` entries, and 76e37025 dropped
  the SDL header that defined it: from index 622 on, hooks and `ORIG_SDL2_*`
  calls were one or two slots off (`SDL_RenderGeometryRaw` called
  `SDL_RenderGeometry`, so no OSD, and `SDL_wcsdup` reached the rumble hook).
  Fixed in 7a83edac.
  To check, read each exported wrapper with `objdump -d`: its `jmp
  *slot(%rip)` gives the real slot, and `slot - index*8` must be the same value
  for all functions.
- `SDL_bool` must stay an `int`: SDL2 games read 32 bits, and a hook returning a
  `bool` only sets the lowest 8, so false could be read as true (`SDL_bool` was
  `bool` from 3905f3b8 on, for SDL3 aliases). An SDL3 function cannot be an alias
  of a `SDL_bool` function without a compiler warning, so it wraps it instead.
  Fixed in 62e2f6cd.
- Opening an SDL game controller doesn't set `joyids`, unlike real SDL which
  opens its joystick. The joystick hooks accept it through
  `mySDL_GameControllerIsOpen()`. Setting `joyids` would be closer to real SDL,
  but would add `SDL_JOY*` events and could desync existing movies, so these
  events are still not generated.
- `utils/simplestsdl12game.c` (SDL 1.2), `utils/simplestgame.c` (SDL2) and
  `utils/simplestsdl3game.c` (SDL3) are minimal games: `gcc
  utils/simplestsdl3game.c -o utils/simplestsdl3game $(pkg-config --cflags --libs
  sdl3)`, or `$(sdl-config --cflags --libs)` for SDL 1.2. The arrow keys move
  the square, so a Lua script calling `input.setKey()` from `callback.onInput()`
  checks that inputs reach the game. Lua `print()` doesn't go to stdout.
- With a fresh `XDG_CONFIG_HOME`, add `-s allow_downloads=0` (or `1`): `-n` still
  shows the libraries download dialog when the setting was never answered, and
  nothing happens, without any window from the game or a line in the output.
- Test in a scratch copy of the game: the game writes next to itself (logs,
  saves) and `LaMulana.bin.x86` is a normal copy of 116 MB. Use a private
  `XDG_CONFIG_HOME` with its own `libTAS.ini` (data, savestate and movie
  directories in scratch), a rootful Xwayland (`Xwayland :43 -geometry
  1600x1200 -decorate -noreset`) and `env -u WAYLAND_DISPLAY QT_QPA_PLATFORM=xcb
  DISPLAY=:43`. See the same section of `wine.md` for the display tips.
- Run with `-n -s fastforward=false -s logging_file=true -s
  logging_console=false -s logging_level=N`. Level 3 is quiet and keeps the
  game fast, level 4 prints every hooked call that matters, level 5 prints
  each call: restrict it with `logging_include_flags` (`LCF_SDL` is `1<<17`,
  OpenGL `1<<15`, sleep `1<<12`, time get `1<<9`, set `1<<8`, see
  `src/shared/lcf.h`) and `logging_exclude_flags` (`1<<26` is the events
  category: `SDL_PollEvent()` alone made a 240 MB log in 14 s).
- Frames: the window title and the libTAS window show the frame count and fps;
  `xwininfo -root -tree` lists the titles, `ffmpeg -f x11grab -video_size WxH
  -i :43+X,Y -frames:v 1 shot.png` takes a screenshot of a part of the test
  display.
- Threads of a running game: `eu-stack -p <pid>` prints the stack of every
  thread, `/proc/<pid>/task/*/comm` and `wchan` the names and what they wait
  on. Sampling a few times in a row tells a thread that sleeps from one that
  spins. Aggregate the samples by thread and by the first frames.
- A crash: `coredumpctl dump <pid> -o core` (the file is not compressed here),
  then `eu-stack --core core -e <binary>` with `-i` for inlined frames; the
  `libtas32.so` frames resolve only if the library has debug info.
- Game state: with symbols in the binary, `objdump -d -M intel
  --no-show-raw-insn` and `nm -C` locate the globals and the functions; read
  the globals of the live process through `/proc/<pid>/mem`.
- Determinism: pause at a fixed frame from Lua (`runtime.playPause()` in
  `onFrame`), then dump `.data` and `.bss` through `/proc/<pid>/mem` (addresses
  from `readelf -S`) and compare two runs word by word, with `nm -CS` to name the
  symbols. Ignore the words that are pointers in both runs. This names the cause
  directly (`first_seed`, `game_timer`...), and it is much stronger than a
  screenshot. A trace of a few globals can hide a lot: the state and the counter
  of La-Mulana do not change with the inputs at all. Crop the screenshots to the
  game window, and expect differences in the FPS of the OSD and in the pointer.
- Lua state is unreliable across savestates: keep "already saved" flags in files.
- Flush Lua files that are watched from outside. Lua buffers its writes, so a
  trace only moves at multiples of 4096 bytes: two full-render runs looked stuck
  at the same frame (4238), which looked like a deterministic hang.
- With `fastforward_render` at its default, the window keeps the last frame that
  was drawn: a pause at frame 1500 showed frame 1465 (the OSD says so). Use
  `fastforward_render=0` to compare pictures, at about 140 fps here instead of
  about 350, or compare memory.
- shell: zsh does not split an unquoted variable into words, so a list of
  `-s` options in a variable reaches libTAS as one argument
  (`Unknown setting key`). Use a shell function with `"$@"`.
- Keep the throttling of heavy games for 3D games: La-Mulana is light.

## Open problems

- Reading the mouse outside of X11 windows on Wayland: see the section "Pointer
  position outside of X11 windows" for what is left (several monitors, rotated
  outputs, Weston, WSLg).
- La-Mulana: one non-draw frame around the handover of the main thread moves
  between frames 280 and 283 (about 1 run out of 4; 6 out of 20 in the last
  count). It happens when the system thread hands over to the logic thread,
  which starts to draw while the system thread is still adding its sleeps. The
  length of the delay is the same, so that the state is identical afterwards
  (checked at frames 1500 and 9000 between runs with a different position), but an input
  on these frames (the first frames of the logo, which take no input as far as
  seen) could be read one game frame apart. Requiring 3 idle scans in a row in
  `waitForIdleThreads()` did not change it. It disappears with debug logging.
- The mode only handles the empty polls of `SDL_PollEvent()`. A pump built on
  another function (`SDL_PeepEvents()`, `XPending()`...) is not detected.
- `sleep_handling=3` was only tested with La-Mulana. A thread that really spins
  makes the main thread wait 5 s at each frame (logged as `still busy`).
- Reading an unchecked result of `createBuffer()` was a crash class in the
  audio hooks (fixed for ALSA, SDL and cubeb, see `wine.md`).
