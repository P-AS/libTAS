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

## Testing and debugging

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
- `utils/simplestgame.c` (SDL2) and `utils/simplestsdl3game.c` (SDL3) are minimal
  games with the SDL renderer: `gcc utils/simplestsdl3game.c -o
  utils/simplestsdl3game $(pkg-config --cflags --libs sdl3)`. The arrow keys move
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
