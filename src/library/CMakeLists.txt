#### Dependencies ####

pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11 x11-xcb)
pkg_check_modules(ALSA REQUIRED IMPORTED_TARGET alsa)

check_include_file_cxx(libudev.h LIBTAS_HAS_LIBUDEV_H)
if(NOT LIBTAS_HAS_LIBUDEV_H)
    message(FATAL_ERROR "The udev headers are required!")
endif()

# swresample and swscale are loaded at runtime, so we only need their headers
foreach(_lib swresample swscale)
    string(TOUPPER ${_lib} _LIB)
    pkg_check_modules(LIB${_LIB} QUIET lib${_lib})
    if(NOT LIB${_LIB}_FOUND)
        message(WARNING "Cannot find lib${_lib} using pkg-config")
        check_include_file_cxx(lib${_lib}/${_lib}.h LIBTAS_HAS_${_LIB}_H)
        if(NOT LIBTAS_HAS_${_LIB}_H)
            message(FATAL_ERROR "The ${_lib} header is required!")
        endif()
    endif()
endforeach()

#### Sources ####

set(LIBTAS_LIBRARY_SOURCES
    backtrace.cpp
    BusyLoopDetection.cpp
    DeterministicTimer.cpp
    FPSMonitor.cpp
    frame.cpp
    GameHacks.cpp
    global.cpp
    GlobalState.cpp
    hook.cpp
    hookpatch.cpp
    logging.cpp
    main.cpp
    NonDeterministicTimer.cpp
    PerfTimer.cpp
    Profiler.cpp
    TimeHolder.cpp
    UnityHacks.cpp
    Utils.cpp
    WindowTitle.cpp
    audio/AudioBuffer.cpp
    audio/AudioContext.cpp
    audio/AudioConverterSwr.cpp
    audio/AudioPlayerAlsa.cpp
    audio/AudioSource.cpp
    audio/DecoderMSADPCM.cpp
    audio/akaudio/akaudio.cpp
    audio/alsa/control.cpp
    audio/alsa/pcm.cpp
    audio/cubeb/cubeb.cpp
    audio/fluidsynth/fluidsynth.cpp
    audio/fmod/fmod.cpp
    audio/fmod/fmodstudio.cpp
    audio/openal/al.cpp
    audio/openal/alc.cpp
    audio/openal/alext.cpp
    audio/openal/alsoft.cpp
    audio/openal/efx.cpp
    audio/pulseaudio/pulseaudio.cpp
    audio/sdl/sdlaudio.cpp
    checkpoint/AltStack.cpp
    checkpoint/Checkpoint.cpp
    checkpoint/CheckpointSavefiles.cpp
    checkpoint/MemArea.cpp
    checkpoint/ProcSelfMaps.cpp
    checkpoint/ReservedMemory.cpp
    checkpoint/SaveStateLoading.cpp
    checkpoint/SaveStateSaving.cpp
    checkpoint/SaveStateManager.cpp
    checkpoint/ThreadLocalStorage.cpp
    checkpoint/ThreadManager.cpp
    checkpoint/ThreadSync.cpp
    encoding/AVEncoder.cpp
    encoding/ImageScalingSws.cpp
    encoding/NutMuxer.cpp
    encoding/Screenshot.cpp
    fileio/dirwrappers.cpp
    fileio/FileDescriptorManip.cpp
    fileio/FileHandleList.cpp
    fileio/generaliowrappers.cpp
    fileio/posixiowrappers.cpp
    fileio/SaveFile.cpp
    fileio/SaveFileList.cpp
    fileio/SaveFileStream.cpp
    fileio/stdiowrappers.cpp
    fileio/URandom.cpp
    general/dlhook.cpp
    general/glibwrappers.cpp
    general/localewrappers.cpp
    general/mallocwrappers.cpp
    general/monowrappers.cpp
    general/pthreadwrappers.cpp
    general/randomwrappers.cpp
    general/signalwrappers.cpp
    general/sleepwrappers.cpp
    general/socketwrappers.cpp
    general/systemwrappers.cpp
    general/timewrappers.cpp
    general/tlswrappers.cpp
    general/waitwrappers.cpp
    inputs/evdev.cpp
    inputs/inputevents.cpp
    inputs/inputs.cpp
    inputs/ioctl_joy.cpp
    inputs/jsdev.cpp
    inputs/keyboard_helper.cpp
    inputs/sdlgamecontroller.cpp
    inputs/sdlhaptic.cpp
    inputs/sdljoystick.cpp
    inputs/sdlkeyboard.cpp
    inputs/sdlkeyboardlayout.cpp
    inputs/sdlpointer.cpp
    inputs/sdltextinput.cpp
    inputs/udevwrappers.cpp
    inputs/winekeyboardlayout.cpp
    inputs/xcbkeyboard.cpp
    inputs/xcbpointer.cpp
    inputs/xkbcommon.cpp
    inputs/xkeyboard.cpp
    inputs/xkeyboardlayout.cpp
    inputs/xinput.cpp
    inputs/xpointer.cpp
    renderhud/AudioDebug.cpp
    renderhud/Crosshair.cpp
    renderhud/FileDebug.cpp
    renderhud/FrameWindow.cpp
    renderhud/InputsWindow.cpp
    renderhud/LogWindow.cpp
    renderhud/LuaDraw.cpp
    renderhud/MessageWindow.cpp
    renderhud/WatchesWindow.cpp
    renderhud/ProfilerDebug.cpp
    renderhud/RenderHUD_GL.cpp
    renderhud/RenderHUD_SDL2_renderer.cpp
    renderhud/RenderHUD_SDL3_renderer.cpp
    renderhud/RenderHUD_Vulkan.cpp
    renderhud/RenderHUD.cpp
    renderhud/RenderHUD_Base_Linux.cpp
    renderhud/UnityDebug.cpp
    rendering/eglwrappers.cpp
    rendering/glxwrappers.cpp
    rendering/openglloader.cpp
    rendering/openglwrappers.cpp
    rendering/vdpauwrappers.cpp
    rendering/vulkanloader.cpp
    rendering/vulkanwrappers.cpp
    screencapture/ScreenCapture_GL.cpp
    screencapture/ScreenCapture_SDL1.cpp
    screencapture/ScreenCapture_SDL2_Renderer.cpp
    screencapture/ScreenCapture_SDL2_Surface.cpp
    screencapture/ScreenCapture_SDL3_Renderer.cpp
    screencapture/ScreenCapture_VDPAU.cpp
    screencapture/ScreenCapture_Vulkan.cpp
    screencapture/ScreenCapture_XShm.cpp
    screencapture/ScreenCapture_Impl.cpp
    screencapture/ScreenCapture.cpp
    sdl/SDLEventQueue.cpp
    sdl/sdldisplay.cpp
    sdl/sdldynapi.cpp
    sdl/sdlevents.cpp
    sdl/sdlmain.cpp
    sdl/sdlmutex.cpp
    sdl/sdlrender.cpp
    sdl/sdlthreads.cpp
    sdl/sdltime.cpp
    sdl/sdltimer.cpp
    sdl/sdlversion.cpp
    sdl/sdlvulkan.cpp
    sdl/sdlwindows.cpp
    steam/CCallback.cpp
    steam/CCallbackManager.cpp
    steam/isteamapps.cpp
    steam/isteamclient/isteamclient.cpp
    steam/isteamclient/isteamclient006.cpp
    steam/isteamclient/isteamclient012.cpp
    steam/isteamclient/isteamclient014.cpp
    steam/isteamclient/isteamclient016.cpp
    steam/isteamclient/isteamclient017.cpp
    steam/isteamclient/isteamclient020.cpp
    steam/isteamclient/isteamclient021.cpp
    steam/isteamcontroller.cpp
    steam/isteamfriends.cpp
    steam/isteamgamecoordinator.cpp
    steam/isteamgameserver.cpp
    steam/isteamhttp.cpp
    steam/isteaminput.cpp
    steam/isteammatchmaking.cpp
    steam/isteamnetworking.cpp
    steam/isteamnetworkingutils.cpp
    steam/isteamnetworkingsockets.cpp
    steam/isteamnetworkingmessages.cpp
    steam/isteamremotestorage/isteamremotestorage.cpp
    steam/isteamremotestorage/isteamremotestorage001.cpp
    steam/isteamremotestorage/isteamremotestorage002.cpp
    steam/isteamremotestorage/isteamremotestorage003.cpp
    steam/isteamremotestorage/isteamremotestorage005.cpp
    steam/isteamremotestorage/isteamremotestorage007.cpp
    steam/isteamremotestorage/isteamremotestorage012.cpp
    steam/isteamremotestorage/isteamremotestorage013.cpp
    steam/isteamremotestorage/isteamremotestorage014.cpp
    steam/isteamremotestorage/isteamremotestorage016.cpp
    steam/isteamscreenshots.cpp
    steam/isteamugc.cpp
    steam/isteamuser/isteamuser.cpp
    steam/isteamuser/isteamuser021.cpp
    steam/isteamuser/isteamuser023.cpp
    steam/isteamuserstats/isteamuserstats.cpp
    steam/isteamuserstats/isteamuserstats005.cpp
    steam/isteamuserstats/isteamuserstats006.cpp
    steam/isteamuserstats/isteamuserstats007.cpp
    steam/isteamuserstats/isteamuserstats008.cpp
    steam/isteamuserstats/isteamuserstats010.cpp
    steam/isteamuserstats/isteamuserstats011.cpp
    steam/isteamuserstats/isteamuserstats012.cpp
    steam/isteamuserstats/isteamuserstats013.cpp
    steam/isteamutils.cpp
    steam/steamapi.cpp
    steam/steamapi_flat.cpp
    steam/steamapiinternal.cpp
    steam/steamgameserver.cpp
    wine/winehook.cpp
    wine/wined3d.cpp
    wine/user32.cpp
    wine/kernel32.cpp
    xcb/XcbEventQueue.cpp
    xcb/XcbEventQueueList.cpp
    xcb/xcbconnection.cpp
    xcb/xcbevents.cpp
    xcb/xcbrequests.cpp
    xcb/xcbwindows.cpp
    xlib/XlibEventQueue.cpp
    xlib/XlibEventQueueList.cpp
    xlib/XlibGameWindow.cpp
    xlib/xatom.cpp
    xlib/xcursor.cpp
    xlib/xdisplay.cpp
    xlib/xevents.cpp
    xlib/xf86vidmode.cpp
    xlib/xfixes.cpp
    xlib/xinerama.cpp
    xlib/xrandr.cpp
    xlib/xshm.cpp
    xlib/xwindows.cpp
    ../shared/inputs/AllInputsFlat.cpp
    ../shared/inputs/ControllerInputs.cpp
    ../shared/inputs/MiscInputs.cpp
    ../shared/inputs/MouseInputs.cpp
    ../shared/inputs/SingleInput.cpp
    ../shared/sockethelpers.cpp
    ../external/lz4.cpp
    ../external/elfhacks.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_demo.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui.cpp
    ../external/imgui/imgui_impl_opengl3.cpp
    ../external/imgui/imgui_impl_sdlrenderer2.cpp
    ../external/imgui/imgui_impl_sdlrenderer3.cpp
    ../external/imgui/imgui_impl_vulkan.cpp
    ../external/imgui/imgui_impl_xlib.cpp
    ../external/imgui/implot_items.cpp
    ../external/imgui/implot.cpp
)

#### Targets ####

function(libtas_add_library target)
    add_library(${target} SHARED ${LIBTAS_LIBRARY_SOURCES})

    set_target_properties(${target} PROPERTIES
        PREFIX "lib"
        NO_SONAME ON
        CXX_VISIBILITY_PRESET hidden)

    target_compile_definitions(${target} PRIVATE LIBTAS_LIBRARY)
    target_compile_options(${target} PRIVATE -fno-stack-protector)
    target_link_options(${target} PRIVATE -rdynamic)

    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}
        ${LIBSWRESAMPLE_INCLUDE_DIRS}
        ${LIBSWSCALE_INCLUDE_DIRS})
    target_compile_options(${target} PRIVATE
        ${LIBSWRESAMPLE_CFLAGS_OTHER}
        ${LIBSWSCALE_CFLAGS_OTHER})

    install(TARGETS ${target} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

if(NOT LIBTAS_I386_LIB_ONLY)
    libtas_add_library(tas)
    target_link_libraries(tas PRIVATE
        PkgConfig::X11
        PkgConfig::ALSA
        Threads::Threads
        ${CMAKE_DL_LIBS})
endif()

if(LIBTAS_BUILD_I386)
    # pkg-config only describes the native libraries, so link the 32-bit ones
    # by name and let the multilib toolchain find them.
    set(LIBTAS_LIBRARY32_LIBS X11 X11-xcb pthread asound ${CMAKE_DL_LIBS})

    foreach(_lib IN LISTS LIBTAS_LIBRARY32_LIBS)
        string(MAKE_C_IDENTIFIER ${_lib} _var)
        cmake_push_check_state(RESET)
        set(CMAKE_REQUIRED_FLAGS -m32)
        set(CMAKE_REQUIRED_LINK_OPTIONS -m32)
        set(CMAKE_REQUIRED_LIBRARIES ${_lib})
        check_cxx_source_compiles("int main() { return 0; }" LIBTAS_HAS_LIB32_${_var})
        cmake_pop_check_state()
        if(NOT LIBTAS_HAS_LIB32_${_var})
            message(FATAL_ERROR "The 32-bit ${_lib} library is required!")
        endif()
    endforeach()

    libtas_add_library(tas32)
    target_compile_options(tas32 PRIVATE -m32)
    target_link_options(tas32 PRIVATE -m32)
    target_include_directories(tas32 PRIVATE ${X11_INCLUDE_DIRS} ${ALSA_INCLUDE_DIRS})
    target_link_libraries(tas32 PRIVATE ${LIBTAS_LIBRARY32_LIBS})
endif()
