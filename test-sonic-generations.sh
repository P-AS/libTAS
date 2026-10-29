#!/bin/bash
# Run Sonic Generations under libTAS through Proton, in a rootful Xwayland
# window managed by openbox, without touching the Steam install or its
# prefix. libTAS uses your own config (~/.config/libTAS): the settings that
# this setup needs are given with -s, which libTAS does not save.
#
# Usage: ./test-sonic-generations.sh [options] [-- libTAS options]
#   --config-tool   run the game's ConfigurationTool.exe instead of libTAS,
#                   under the same conditions as the game (to regenerate
#                   GraphicsConfig.cfg: delete it first, then Save and Quit)
#   --reset         delete the test directory and set it up again
#   --restart-x     restart the test Xwayland and its openbox (clears windows
#                   left by killed games)
#
# Environment (defaults in brackets):
#   GAME_DIR    [~/.local/share/Steam/steamapps/common/Sonic Generations]
#   PROTON_DIR  [~/Steam/steamapps/common/Proton 11.0]
#   TEST_DIR    [~/.local/share/libTAS-test/sonic-generations]
#   TEST_DISPLAY [:42]
#   TEST_SIZE   [1600x1000] size of the test display. Under niri, its window
#               is made floating at that size; otherwise resize it yourself.
#
# See the Proton section of wine.md for why each setup step is needed.

set -e

LIBTAS_DIR="$(cd "$(dirname "$0")" && pwd)"
GAME_DIR="${GAME_DIR:-$HOME/.local/share/Steam/steamapps/common/Sonic Generations}"
PROTON_DIR="${PROTON_DIR:-$HOME/Steam/steamapps/common/Proton 11.0}"
TEST_DIR="${TEST_DIR:-${XDG_DATA_HOME:-$HOME/.local/share}/libTAS-test/sonic-generations}"
TEST_DISPLAY="${TEST_DISPLAY:-:42}"
TEST_SIZE="${TEST_SIZE:-1600x1000}"
STEAM_CLIENT_DIR="${STEAM_CLIENT_DIR:-$HOME/.local/share/Steam}"
APPID=71340

CONFIG_TOOL=0
RESET=0
RESTART_X=0
while [ $# -gt 0 ]; do
    case "$1" in
        --config-tool) CONFIG_TOOL=1 ;;
        --reset) RESET=1 ;;
        --restart-x) RESTART_X=1 ;;
        --) shift; break ;;
        -h|--help) sed -n '2,20p' "$0"; exit 0 ;;
        *) echo "Unknown option $1 (libTAS options go after --)"; exit 1 ;;
    esac
    shift
done

die() { echo "error: $*" >&2; exit 1; }

[ -f "$GAME_DIR/SonicGenerations.exe" ] || die "game not found in $GAME_DIR (set GAME_DIR)"
[ -x "$PROTON_DIR/proton" ] || die "Proton not found in $PROTON_DIR (set PROTON_DIR)"
LIBTAS="$LIBTAS_DIR/build/src/program/libTAS"
[ -x "$LIBTAS" ] || die "libTAS is not built ($LIBTAS)"

# The launcher injects the libtas.so next to it, which a cmake build does not update
if [ "$LIBTAS_DIR/build/src/library/libtas.so" -nt "$LIBTAS_DIR/build/src/program/libtas.so" ]; then
    echo "Updating build/src/program/libtas.so"
    cp "$LIBTAS_DIR/build/src/library/libtas.so" "$LIBTAS_DIR/build/src/program/libtas.so"
fi

[ $RESET -eq 1 ] && rm -rf "$TEST_DIR"

TGAME="$TEST_DIR/game"
COMPAT="$TEST_DIR/compat"
PFX="$COMPAT/pfx"
mkdir -p "$TGAME" "$COMPAT"

export STEAM_COMPAT_DATA_PATH="$COMPAT"
export STEAM_COMPAT_CLIENT_INSTALL_PATH="$STEAM_CLIENT_DIR"
export SteamAppId=$APPID SteamGameId=$APPID
export PROTON_USE_WOW64=1

### Test display

if [ $RESTART_X -eq 1 ]; then
    pkill -f "^Xwayland $TEST_DISPLAY " || true
    sleep 1
fi
if ! DISPLAY=$TEST_DISPLAY xdpyinfo >/dev/null 2>&1; then
    echo "Starting rootful Xwayland on $TEST_DISPLAY"
    Xwayland $TEST_DISPLAY -geometry $TEST_SIZE -decorate -noreset >"$TEST_DIR/xwayland.log" 2>&1 &
    for _ in $(seq 1 20); do
        DISPLAY=$TEST_DISPLAY xdpyinfo >/dev/null 2>&1 && break
        sleep 0.5
    done
fi

# A tiling compositor sizes the rootful Xwayland window, and so the X screen,
# to whatever fits its layout. Under niri, make it floating at TEST_SIZE.
if [ -n "$NIRI_SOCKET" ] && command -v niri >/dev/null; then
    niri_id=$(niri msg --json windows | python3 -c '
import json, sys
for w in json.load(sys.stdin):
    if w.get("title") == "Xwayland on " + sys.argv[1]:
        print(w["id"], "floating" if w.get("is_floating") else "tiled")
        break' "$TEST_DISPLAY")
    if [ -n "$niri_id" ]; then
        [ "${niri_id#* }" = "tiled" ] && niri msg action toggle-window-floating --id "${niri_id% *}"
        niri msg action set-window-width --id "${niri_id% *}" "${TEST_SIZE%x*}"
        niri msg action set-window-height --id "${niri_id% *}" "${TEST_SIZE#*x}"
        sleep 0.5
    fi
fi

# Wine reports the modes of the X screen, and the game refuses to start if
# the resolution of GraphicsConfig.cfg (1280x720) is not one of them
display_size() { DISPLAY=$TEST_DISPLAY xdpyinfo | awk '/dimensions:/ {print $2}'; }
while true; do
    size=$(display_size)
    w=${size%x*}; h=${size#*x}
    if [ "$w" -ge 1280 ] && [ "$h" -ge 720 ] && [ "$w" -ge "$h" ]; then
        break
    fi
    echo "The $TEST_DISPLAY window is ${size}: make it landscape and at least 1280x720 (float or resize it). Waiting..."
    sleep 3
done
echo "Display $TEST_DISPLAY is $size"

# Window manager, so that the game gets the focus like on a desktop
# (_NET_ACTIVE_WINDOW). Only on the test display.
if ! DISPLAY=$TEST_DISPLAY xprop -root _NET_SUPPORTING_WM_CHECK 2>/dev/null | grep -q 'window id'; then
    echo "Starting openbox on $TEST_DISPLAY"
    env -u WAYLAND_DISPLAY DISPLAY=$TEST_DISPLAY openbox >"$TEST_DIR/openbox.log" 2>&1 &
    for _ in $(seq 1 20); do
        DISPLAY=$TEST_DISPLAY xprop -root _NET_SUPPORTING_WM_CHECK 2>/dev/null | grep -q 'window id' && break
        sleep 0.5
    done
fi

run_proton() {
    env -u WAYLAND_DISPLAY DISPLAY=$TEST_DISPLAY WINEDEBUG=-all "$PROTON_DIR/proton" run "$@"
}

### Game directory: copies of the small files, links to the big data directories

if [ ! -f "$TGAME/SonicGenerations.exe" ]; then
    echo "Setting up the game directory in $TGAME"
    for f in "$GAME_DIR"/*; do
        b=$(basename "$f")
        case "$b" in
            disk|movie|Sound|redist|work) ln -sfn "$f" "$TGAME/$b" ;;
            GraphicsConfig.cfg) ;;
            *) cp -p "$f" "$TGAME/$b" ;;
        esac
    done
fi

# Graphics config matching what wined3d with software rendering reports
# (the one of the Steam install matches DXVK and the real GPU)
if [ ! -f "$TGAME/GraphicsConfig.cfg" ]; then
    echo "Writing a wined3d GraphicsConfig.cfg (1280x720)"
    printf '%s\r\n' \
        "Do not manually edit this file, use the configuration tool." \
        "NVIDIA GeForce GTX 470" \
        '\\.\DISPLAY1' \
        "1280.720.60" \
        0 1 0 0 0 \
        "aeb2cdd4-6e41-43ea-941c-8361cc760781" \
        "" \
        "1515474505" > "$TGAME/GraphicsConfig.cfg"
fi

### Proton prefix

# Registry key that Steam's install script creates, all in one Proton run
if [ ! -f "$TEST_DIR/.prefix-ready" ]; then
    echo "Creating the Proton prefix in $PFX (takes a while)"
    REGGAME="Z:$(echo "$TGAME" | sed 's#/#\\\\#g')"
    printf '%s\r\n' \
        'REGEDIT4' \
        '' \
        '[HKEY_LOCAL_MACHINE\Software\Wow6432Node\Sega\Sonic Generations]' \
        "\"install_path\"=\"$REGGAME\"" \
        "\"exe_path\"=\"$REGGAME\\\\SonicGenerations.exe\"" \
        '"savelocation"="My Games\\Sonic Generations\\Saved Games"' \
        '"locale"="1033"' > "$TEST_DIR/sonic-generations.reg"
    run_proton regedit /S "Z:$(echo "$TEST_DIR/sonic-generations.reg" | tr '/' '\\')"
    touch "$TEST_DIR/.prefix-ready"
fi

# Keyboard bindings: the configuration tool writes them all as 0 under Wine,
# so take the defaults of the game
SAVEDIR="$PFX/drive_c/users/steamuser/Documents/My Games/Sonic Generations/Saved Games"
mkdir -p "$SAVEDIR"
if [ ! -f "$SAVEDIR/PlayerInput.cfg" ] || grep -q '\$B:0 0 0 0' "$SAVEDIR/PlayerInput.cfg"; then
    echo "Writing keyboard bindings from DefaultInput.cfg"
    head -n 2 "$TGAME/DefaultInput.cfg" > "$SAVEDIR/PlayerInput.cfg"
fi

cd "$TGAME"

if [ $CONFIG_TOOL -eq 1 ]; then
    PROTON_USE_WINED3D=1 LIBGL_ALWAYS_SOFTWARE=1 __GLX_VENDOR_LIBRARY_NAME=mesa \
        run_proton "$TGAME/ConfigurationTool.exe"
    exit 0
fi

### libTAS, with your config: settings given with -s are not saved.
# opengl_soft must match the adapter of GraphicsConfig.cfg.

echo "Starting libTAS. The game starts paused before its first frame, so its"
echo "window stays black until you press Pause. Keys on the game window: Pause"
echo "play/pause, v frame advance, Tab fast-forward, Shift+F1/F1 save/load state 1,"
echo "Return Start, WASD/arrows"
exec env -u WAYLAND_DISPLAY QT_QPA_PLATFORM=xcb DISPLAY=$TEST_DISPLAY \
    "$LIBTAS" \
    -s use_proton=true -s "proton_path=$PROTON_DIR" -s opengl_soft=true \
    "$@" SonicGenerations.exe
