#!/bin/sh

set -e
cd "$(dirname "$0")"

SKIP_APPIMAGE=0
if [ "$1" = "--skip-appimage" ]; then
    SKIP_APPIMAGE=1
    shift
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
    wget -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-$ARCH.AppImage
    chmod +x linuxdeploy-$ARCH.AppImage
    wget -nc https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-$ARCH.AppImage
    chmod +x linuxdeploy-plugin-qt-$ARCH.AppImage
fi

APPDIR_PATH=$(readlink -f ./AppDir)

DESTDIR=$APPDIR_PATH cmake --install . --prefix /usr
if [ $SKIP_APPIMAGE -eq 0 ]; then
    ./linuxdeploy-$ARCH.AppImage --appimage-extract-and-run --appdir AppDir/ --plugin qt --output appimage
fi
