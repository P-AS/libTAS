---
layout: page
title: Rayman 2 with Wine
permalink: /guides/rayman2/
---

This page explains how to run the GOG version of Rayman 2: The Great Escape
under libTAS through Wine on Linux, with the
[Ray2Fix](https://github.com/spitfirex86/Ray2Fix) collection of fixes.

* TOC
{:toc}

## Requirements

* libTAS built from source on the `wine` branch, so that `libTAS`, `libtas.so`
  and the `libtas-xinput*.dll` files are in `build/src/program`. The xinput
  dlls need the mingw-w64 cross compilers (`i686-w64-mingw32-gcc`).
* Wine 11 (tested with 11.21, as packaged by Arch Linux). Proton is not needed.
* The GOG version of Rayman 2, installed with the standalone installer.
* [Ray2Fix](https://github.com/spitfirex86/Ray2Fix/releases) v1.4.3.
* [dgVoodoo2](https://dege.freeweb.hu/dgVoodoo2/) version 2.79.3, for example
  from
  [this archive](https://github.com/legluondunet/mlls-tools/raw/refs/heads/master/dgVoodoo2/dgVoodoo2_79_3.zip).

Throughout this page, `$GAME` is the game directory, for example:

```
GAME="$HOME/Games/Rayman 2"
```

## 1. Install Ray2Fix and dgVoodoo2

1. Extract all the files of the Ray2Fix release into `$GAME`, including the
   `DLL` folder, and overwrite the files of the game.
2. Replace the dgVoodoo2 files that Ray2Fix ships with the files of the same
   name from dgVoodoo2 2.79.3: `Glide2x.dll` (from the `3Dfx/x86` folder of the
   archive), `DDraw.dll` (from `MS/x86`) and `dgVoodooCpl.exe`. The tested
   setup kept the `D3DImm.dll` of Ray2Fix.
3. Remove the `glide2x.dll` of the GOG version. Linux file names are case
   sensitive, so it stays next to dgVoodoo's `Glide2x.dll`, and Wine may load
   either of them. Renaming it (for example to `glide2x.dll.bak`) is enough.

Other files exist in two cases after this (`3dfxspl2.dll` and `3DfxSpl2.dll`,
`ddraw.dll` and `DDraw.dll`). The setup of this page was tested with both
present.

Run `R2FixCfg.exe` with Wine to choose the display settings if you want to
change them. In its Input tab, keep the XInput controller backend: the SDL3
backend reads the controllers of the system directly, which libTAS does not
control.

## 2. Configure libTAS

Open `$GAME/Rayman2.exe` in libTAS and set:

* **Environment** (Executable Options): `WINEDLLOVERRIDES` to
  `dinput8,dinput=n,b`, so that Wine loads the input dlls of Ray2Fix (Xidi)
  instead of its own.
* **Settings > Runtime**: frames per second 60.
* **Settings > Video**: software OpenGL (`opengl_soft`) was used for testing.
* **Settings > Debug**: leave "Keep main first thread" unchecked. Rayman 2 runs
  its engine on a second thread, which libTAS must use as the main thread.
* **Settings > Game-specific**: leave the "Idle Threads" sync unchecked. It
  does not make Rayman 2 more deterministic, and slows it down.
* Do not enable wined3d's CSMT (`csmt` in `WINE_D3D_CONFIG`). libTAS disables
  it by default.

The same settings from the command line:

```
cd "$GAME"
libTAS -s fastforward=false -s framerate_num=60 -s framerate_den=1 \
    -s game_specific_sync=0 Rayman2.exe
```

The environment variable is set from Executable Options, or by starting libTAS
with `WINEDLLOVERRIDES="dinput8,dinput=n,b"` already in its environment.

## 3. Inputs

* **Keyboard**: the game reads the keyboard with DirectInput. libTAS gives it
  the keys of the current frame, so key presses of a movie always reach the
  same frame.
* **Controllers**: Xidi turns XInput controllers into the DirectInput
  controllers that the game reads, and libTAS gives its own controllers to
  XInput. Set the number of controllers in the input settings. The controllers
  of your system are hidden from the game. Controllers were not tested with
  Rayman 2 yet.

## What works

Tested in the language and main menus and the New Game screens (each run
under 30 seconds, at normal speed):

* The game starts and runs at 60 fps. The non-draw frames at startup come from
  a one-second `Sleep()` of the game, the same in every run.
* Two runs with the same inputs give the same image at the same frame (frames
  300 and 1100 compared pixel for pixel).
* Savestates: 11 loads in a row of the same state, and a run that saves at
  frame 600 and loads it at frame 900 gives the same image at frame 1100 as a
  run without loading.

Not tested yet: gameplay in a level, long movies, controllers.

## Troubleshooting

* If the game is not deterministic, check that "Keep main first thread" is
  off, and that the log shows `Wine game draws from thread ..., making it the
  main thread` (thread logging at info level), about 30 frames after start.
* The Ray2Fix window title shows the real frame rate, which varies. It is not
  part of the game picture.
