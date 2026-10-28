# Windows steam_api.dll replacements, which libtas loads in Wine games instead
# of their own when the virtual Steam client is enabled. They are built from
# the sources of the virtual Steam client with the mingw-w64 cross compilers,
# and skipped if those are missing.

find_program(LIBTAS_MINGW32_CXX i686-w64-mingw32-g++)
find_program(LIBTAS_MINGW64_CXX x86_64-w64-mingw32-g++)

file(GLOB_RECURSE _steam_sources CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/library/steam/*.cpp)
file(GLOB_RECURSE _steam_headers CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/library/steam/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/compat/*.h)
set(_steamdll_sources ${_steam_sources} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(_steamdll_flags
    -std=c++17 -O2 -shared
    -I${CMAKE_CURRENT_SOURCE_DIR}/compat
    -I${PROJECT_SOURCE_DIR}/src/library
    -static -static-libgcc -static-libstdc++)

# Put them next to the launcher and libtas.so, like when installed
set(_steamdll_dir ${PROJECT_BINARY_DIR}/src/program)

set(_steamdll_outputs)
foreach(_arch 32 64)
    if(_arch EQUAL 32)
        set(_cxx ${LIBTAS_MINGW32_CXX})
        set(_dll libtas-steam_api.dll)
    else()
        set(_cxx ${LIBTAS_MINGW64_CXX})
        set(_dll libtas-steam_api64.dll)
    endif()

    if(NOT _cxx)
        message(STATUS "mingw-w64 compiler for ${_arch}-bit Windows not found, the virtual Steam client will not work for ${_arch}-bit Wine games")
        continue()
    endif()

    add_custom_command(
        OUTPUT ${_steamdll_dir}/${_dll}
        COMMAND ${_cxx} ${_steamdll_flags} ${_steamdll_sources} -o ${_steamdll_dir}/${_dll}
        DEPENDS ${_steamdll_sources} ${_steam_headers}
        COMMENT "Building ${_dll}"
        VERBATIM)
    list(APPEND _steamdll_outputs ${_steamdll_dir}/${_dll})
endforeach()

if(_steamdll_outputs)
    add_custom_target(steamdll ALL DEPENDS ${_steamdll_outputs})
    install(FILES ${_steamdll_outputs} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
