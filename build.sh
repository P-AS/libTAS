#!/bin/sh

set -e
cd "$(dirname "$0")"

SKIP_APPIMAGE=0
if [ "$1" = "--skip-appimage" ]; then
    SKIP_APPIMAGE=1
    shift
fi

# AppImage only works on Linux
if [ "$(uname -s)" != "Linux" ]; then
    SKIP_APPIMAGE=1
fi

# Translate the former autotools options, and pass everything else to cmake.
# Options are always set explicitly, because cmake caches them between runs.
BUILD_I386=OFF
I386_LIB_ONLY=OFF
RELEASE_BUILD=OFF
BUILD_DATE=ON
CMAKE_ARGS=""
for arg in "$@"; do
    case "$arg" in
        --with-i386) BUILD_I386=ON ;;
        --enable-i386-lib) I386_LIB_ONLY=ON ;;
        --enable-release-build) RELEASE_BUILD=ON ;;
        --disable-build-date) BUILD_DATE=OFF ;;
        *) CMAKE_ARGS="$CMAKE_ARGS $arg" ;;
    esac
done

CMAKE_ARGS="-DLIBTAS_BUILD_I386=$BUILD_I386 -DLIBTAS_I386_LIB_ONLY=$I386_LIB_ONLY -DLIBTAS_RELEASE_BUILD=$RELEASE_BUILD -DLIBTAS_BUILD_DATE=$BUILD_DATE $CMAKE_ARGS"

CXXFLAGS="-O2 -g -Wall -pedantic" cmake -S . -B build $CMAKE_ARGS
cmake --build build -j$(nproc)
cd build

# Build .AppImage

ARCH=$(uname -m)
if [ $SKIP_APPIMAGE -eq 0 ]; then
    [ -f linuxdeploy-$ARCH.AppImage ] || wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-$ARCH.AppImage
    chmod +x linuxdeploy-$ARCH.AppImage
    [ -f linuxdeploy-plugin-qt-$ARCH.AppImage ] || wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-$ARCH.AppImage
    chmod +x linuxdeploy-plugin-qt-$ARCH.AppImage
else
    echo "Skipping AppImage build (not running on Linux)"
fi

APPDIR_PATH=$(readlink -f ./AppDir)

DESTDIR=$APPDIR_PATH cmake --install . --prefix /usr
if [ $SKIP_APPIMAGE -eq 0 ]; then
    # Point the qt plugin to the qmake of the Qt version that was built against
    if [ -z "$QMAKE" ] && grep -q '^LIBTAS_QT_VERSION_USED:INTERNAL=6$' CMakeCache.txt; then
        export QMAKE=$(command -v qmake6 || command -v qmake-qt6 || echo qmake)
    fi
    # Only the xcb platform plugin is bundled by default, add the wayland one if installed
    QT_PLATFORMS_DIR="$(${QMAKE:-qmake} -query QT_INSTALL_PLUGINS)/platforms"
    if ls "$QT_PLATFORMS_DIR"/libqwayland*.so >/dev/null 2>&1; then
        export EXTRA_PLATFORM_PLUGINS=$(cd "$QT_PLATFORMS_DIR" && ls libqwayland*.so | paste -sd ';')
    else
        echo "Qt wayland platform plugin not found, the AppImage will only support X11"
    fi
    # The strip bundled with linuxdeploy is too old for libraries using .relr.dyn
    export NO_STRIP=1
    ./linuxdeploy-$ARCH.AppImage --appimage-extract-and-run --appdir AppDir/ --plugin qt --output appimage
fi
