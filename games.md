# Running native Linux games

This document records what was learned while testing native Linux games under
libTAS: problems found, how they were fixed, design choices, and what is still
open. It is the equivalent of `wine.md` for games that are not Windows
programs. Keep it up to date when working on anything native-game related
(see `AGENTS.md`). Wine specific problems stay in `wine.md`.

Tested setup: Arch Linux, Mesa llvmpipe (`opengl_soft=true`), rootful Xwayland
display, libTAS built from `next` with `cmake`. Games used for testing:

- **La-Mulana** (Steam app 230700, Nigoro, version 1.5.5.2, 32-bit ELF, its
  own SDL2, OpenAL and `libsteam_api.so` in `lib/`, OpenGL 2). See
  [La-Mulana](#la-mulana).

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
- libTAS has one "main thread": the thread that last reached a frame boundary
  (a buffer swap, or enough accumulated sleep). The frame count, the
  deterministic time and the audio mixing advance only at its boundaries.
  Which threads advance game time with their sleeps is the `sleep_handling`
  setting (`0` never, `1` only the main thread, `2` every thread). The
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
| `sleep_handling=2` | frames advance (1400 frames in 26 s, the system thread triggers them with its sleeps), 3 of them draws, then a still picture with a few blue marks (checked over 40 s); state 3, `initDraw` never draws |
| `sleep_handling=2` with Idle Threads sync (`game_specific_sync=64`), or native events (`debug_state=2`), or `async_events=3` | same as above |
| `sleep_handling=0` | `initDraw` runs (`0x89688c4 = 2`), but the frame counter stays at 1 (checked 8 s apart): stuck in state 3 |
| `debug_state=1` (uncontrolled time), with or without native events | works: state 4, the intro text ("It is where I belong. Children, please help"), 150 to 250 fps, frame counter counts draws |

Reasons, from the traces:

- With the default `sleep_handling=1` only sleeps of the main thread advance
  time, and the main thread is the first thread (it never sleeps) until some
  thread reaches a frame boundary. The game threads wait for `SDL_GetTicks()`
  to move, and it does not.
- With `sleep_handling=2` the system thread (`mySleep()`) becomes the main
  thread and makes frames, but the game still does not leave state 3. The
  logic thread is in the `OSSleepTicks(500)` loop, and `initDraw` never
  reaches its drawing branch. Why was not found: its early returns depend on
  `dsystemFlag()` values (8, `0x10` and 1, kept in memory that I did not
  read), and which one applies was not checked. This is the open question.
- The trace of the working (uncontrolled) run shows the two drawing threads,
  with `SDL_GetTicks()` called 56 000 times in 766 frames by the thread that
  draws (a spin on the clock), and 1100 `glXMakeCurrent()` pairs.

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

### Steam

`main()` calls `SteamAPI_Init()` and shows a message box when it fails
("Steam must be running to play this game" was reported by the user). Use
`virtual_steam=true` to avoid needing the client. The 32-bit
`libsteam_api.so` of the game is what the virtual client replaces.

### Not done / not checked

- Inputs: no key or controller was sent to the game.
- Savestates, movies and determinism: not tested, and there is no
  deterministic configuration yet.
- Whether the native events setting matters for input in the uncontrolled run
  (the page says it is needed; the game runs without it).
- The shutdown crash described in Open problems.

## Testing and debugging

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
- shell: zsh does not split an unquoted variable into words, so a list of
  `-s` options in a variable reaches libTAS as one argument
  (`Unknown setting key`). Use a shell function with `"$@"`.
- Keep the throttling of heavy games for 3D games: La-Mulana is light.

## Open problems

- La-Mulana does not leave its loading state with controlled time (see the
  table above). Next steps: find out which `dsystemFlag()` keeps `initDraw`
  from drawing under `sleep_handling=2` (read the flag storage in the
  process), and whether the logic thread waits on the system thread or the
  other way around; try giving the first thread a frame boundary when it
  spins on `SDL_PollEvent()` (libTAS' busy loop detection only counts calls
  to the time functions).
- Crash at exit with debug logging: during `_dl_fini`, the OpenAL teardown of
  La-Mulana calls the hooked `pthread_key_delete()`, whose `link_function()`
  logs through `LogWindow::addLog()` into an ImGui text buffer that is already
  destroyed (`ImGuiTextBuffer::append`, SIGSEGV in `memcpy`). It only happens
  when the game exits with a logging level that prints that call.
- Reading an unchecked result of `createBuffer()` was a crash class in the
  audio hooks (fixed for ALSA, SDL and cubeb, see `wine.md`).
