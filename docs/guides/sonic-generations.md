---
layout: page
title: Sonic Generations with Proton
permalink: /guides/sonic-generations/
---

This page explains how to run Sonic Generations (Steam version) under libTAS
through Proton on Linux. The steps use a separate game directory and Proton
prefix, so your Steam install and its prefix are left alone.

* TOC
{:toc}

## Requirements

* libTAS built from source, so that `libTAS` and `libtas.so` are in
  `build/src/program`.
* Sonic Generations installed through Steam.
* Proton 11.0 installed through Steam (other versions were not tested).
* A rootful Xwayland display for the game. libTAS does not work with the game
  on a Wayland session, so the game runs on its own X display (`:42` below).
* `Xwayland`, `xdpyinfo`, `openbox` and `xprop` installed.

Throughout this page, `$GAME` is the Steam install of the game, and `$PROTON`
is the Proton installation:

```
GAME="$HOME/.local/share/Steam/steamapps/common/Sonic Generations"
PROTON="$HOME/Steam/steamapps/common/Proton 11.0"
TEST="$HOME/.local/share/libTAS-test/sonic-generations"
```

Adjust them if your Steam library is somewhere else.

## 1. Start a display for the game

Start a rootful Xwayland on `:42` at 1600x1000, and a window manager on it so
that the game gets the focus like on a desktop:

```
mkdir -p "$TEST"
Xwayland :42 -geometry 1600x1000 -decorate -noreset >"$TEST/xwayland.log" 2>&1 &
env -u WAYLAND_DISPLAY DISPLAY=:42 openbox >"$TEST/openbox.log" 2>&1 &
```

The game needs a landscape display of at least 1280x720. A tiling window
manager sizes the Xwayland window to fit its layout, which can give a portrait
screen, and the game then refuses to start. Check the size with:

```
DISPLAY=:42 xdpyinfo | grep dimensions
```

Under [niri](https://github.com/YaLTeR/niri), make the `Xwayland on :42` window
floating and size it:

```
niri msg --json windows   # find the id of the "Xwayland on :42" window
niri msg action toggle-window-floating --id <id>
niri msg action set-window-width --id <id> 1600
niri msg action set-window-height --id <id> 1000
niri msg action center-window --id <id>
```

With another compositor, float or resize that window yourself.

## 2. Set up the game directory

Make a directory for the game. Copy the small files, and link the big data
directories instead of copying them:

```
mkdir -p "$TEST/game"
cd "$GAME"
for f in *; do
    case "$f" in
        disk|movie|Sound|redist|work) ln -sfn "$GAME/$f" "$TEST/game/$f" ;;
        GraphicsConfig.cfg) ;;
        *) cp -p "$f" "$TEST/game/$f" ;;
    esac
done
```

### Graphics configuration

The game checks `GraphicsConfig.cfg` against the graphics adapter it sees. Under
this setup, Direct3D 9 is provided by wined3d with software rendering, so the
file must describe that adapter. Write it with CRLF line endings, in
`$TEST/game/GraphicsConfig.cfg`:

```
Do not manually edit this file, use the configuration tool.
NVIDIA GeForce GTX 470
\\.\DISPLAY1
1280.720.60
0
1
0
0
0
aeb2cdd4-6e41-43ea-941c-8361cc760781

1515474505
```

The lines are the adapter name, the monitor id, the resolution with its refresh
rate, a few flags, the device GUID, and an empty line and a number at the end.
The number is the FOURCC `INTZ`, not a checksum.

If the game still reports that the configuration does not match your hardware,
regenerate the file with the game's configuration tool, under the same
conditions as the game. Delete `GraphicsConfig.cfg` first, then run:

```
cd "$TEST/game"
PROTON_USE_WINED3D=1 LIBGL_ALWAYS_SOFTWARE=1 __GLX_VENDOR_LIBRARY_NAME=mesa \
    DISPLAY=:42 WINEDEBUG=-all "$PROTON/proton" run ConfigurationTool.exe
```

Choose the settings, then Save and Quit. Keep the resolution at 1280x720 (or
another mode the display reports).

## 3. Create the Proton prefix

Export the environment that Proton needs:

```
export STEAM_COMPAT_DATA_PATH="$TEST/compat"
export STEAM_COMPAT_CLIENT_INSTALL_PATH="$HOME/.local/share/Steam"
export SteamAppId=71340 SteamGameId=71340
export PROTON_USE_WOW64=1
mkdir -p "$STEAM_COMPAT_DATA_PATH"
```

Use a new compatdata directory, not the one of another Proton (see the
[Wine notes](https://github.com/clementgallet/libTAS/blob/next/wine.md#proton)).

The game reads its install location from the registry, which Steam's install
script normally writes. Create the same keys yourself, in
`$TEST/sonic-generations.reg`. Replace the path with the Windows form of
`$TEST/game` (`Z:` followed by the path with backslashes):

```
REGEDIT4

[HKEY_LOCAL_MACHINE\Software\Wow6432Node\Sega\Sonic Generations]
"install_path"="Z:\\home\\you\\.local\\share\\libTAS-test\\sonic-generations\\game"
"exe_path"="Z:\\home\\you\\.local\\share\\libTAS-test\\sonic-generations\\game\\SonicGenerations.exe"
"savelocation"="My Games\\Sonic Generations\\Saved Games"
"locale"="1033"
```

Then import it with Proton:

```
DISPLAY=:42 WINEDEBUG=-all env -u WAYLAND_DISPLAY "$PROTON/proton" run \
    regedit /S "Z:$TEST/sonic-generations.reg"
```

The `locale` value is required. Without it the game shows
`Game files either missing or corrupt please reinstall`.

## 4. Set the keyboard bindings

The configuration tool saves all keyboard bindings as 0 under Wine, and the
game then ignores the keyboard. Copy the first two lines of the game's
`DefaultInput.cfg` into the save directory of the prefix:

```
SAVEDIR="$STEAM_COMPAT_DATA_PATH/pfx/drive_c/users/steamuser/Documents/My Games/Sonic Generations/Saved Games"
mkdir -p "$SAVEDIR"
head -n 2 "$TEST/game/DefaultInput.cfg" > "$SAVEDIR/PlayerInput.cfg"
```

## 5. Optional: the Direct3D 11 mod

The GenerationsD3D11 mod (Direct3D 11 renderer) goes in the `mods` folder of
the game directory:

```
mkdir -p "$TEST/game/mods"
cp -rp GenerationsD3D11 "$TEST/game/mods/"
```

Set `DisplayMode` in `mods/GenerationsD3D11/GenerationsD3D11.ini`: `0` is
borderless, `1` borderless fullscreen, `2` windowed.

The mod ships its own `D3DCompiler_47.dll`. Wine needs the override
`d3dcompiler_47=n,b` to load it instead of its built-in one, which is why the
launch command below sets `WINEDLLOVERRIDES`. This is the only override the
mod's files need on their own. Its `d3d9.dll`, `d3d11.dll` and `dxgi.dll`
imports are Wine's built-in DLLs, and `ShaderTranslator.dll` imports
`mscoree.dll` (.NET), which Proton may not provide, so this is not verified.

## 6. Start libTAS

From the libTAS source directory, start libTAS with your own configuration, on
the game display, with Proton:

```
cd "$TEST/game"
env -u WAYLAND_DISPLAY QT_QPA_PLATFORM=xcb DISPLAY=:42 \
    WINEDLLOVERRIDES="d3dcompiler_47=n,b;winebus.sys=d" \
    /path/to/libTAS/build/src/program/libTAS \
    -s use_proton=true -s "proton_path=$PROTON" -s opengl_soft=true \
    -s game_specific_sync=64 \
    SonicGenerations.exe
```

`-s` settings are not saved to your libTAS configuration. `opengl_soft` must
stay on, as it matches the adapter of `GraphicsConfig.cfg`. Other libTAS options
can be added before `SonicGenerations.exe`.

- `winebus.sys=d` disables Wine's controller support. Without it, loading a
  savestate can crash the game, because a controller read in progress when the
  state was saved gets completed twice. libTAS does not give controller inputs
  to Windows games yet, so nothing is lost.
- Keep the virtual Steam client of libTAS (the default setting). The Steam
  client of Proton (`-s virtual_steam=false`) talks to the real Steam process
  in real time, so two runs of the same movie play differently, and a movie
  recorded with it does not stay in sync. Movies must be recorded with the
  virtual Steam client.
- `game_specific_sync=64` is the "Idle Threads Sync" option, in the
  game-specific settings of libTAS. It makes the loadings of the game play the
  same way on every run. Record your movies with it enabled too: a movie
  recorded without it does not stay in sync with it, and the other way around.

The game starts paused before its first frame, so its window stays black until
you press Pause.

### Controls

On the game window:

| Key | Action |
| --- | --- |
| Pause | Play and pause |
| `v` | Frame advance |
| Tab | Fast-forward |
| Shift+F1 / F1 | Save / load state 1 |
| Return | Start |
| WASD / arrows | Movement |

## Troubleshooting

* **`could not load kernel32.dll`**: the prefix was created by another Proton.
  Use a new `STEAM_COMPAT_DATA_PATH`.
* **The game window is black or cropped**: the display size does not match the
  configuration. Check step 1 and 2.
* **Keys do nothing**: check step 4.
* **Killed game, windows left on `:42`**: restart the Xwayland and openbox from
  step 1, since the old windows have the same names as the new ones.

## Known limitations

* Under software rendering the game runs at about 25 frames per second in real
  time.
* Only Proton 11.0 has been tested.
* The first stage plays the same way on repeated runs of a movie (same game
  timer and position at frame 1500 in 6 of 6 runs), but a rare divergence in
  the first 30 frames remains (about 1 run in 10 to 20), and about 0.1% of the
  pixels of a frame differ between runs. Later stages have not been checked.
