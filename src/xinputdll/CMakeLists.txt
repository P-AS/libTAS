# Windows xinput dlls, which libtas loads in Wine games instead of the ones of
# Wine so that games get the controllers of libtas. They are built with the
# mingw-w64 cross compilers, and skipped if those are missing.
#
# Each file name has the exports of one version of the dll (old games import
# the functions by ordinal). libtas gives the loader the one that matches the
# name of the dll that the game asked for.

find_program(LIBTAS_MINGW32_CC i686-w64-mingw32-gcc)
find_program(LIBTAS_MINGW64_CC x86_64-w64-mingw32-gcc)

set(_xinput_versions xinput1_1 xinput1_2 xinput1_3 xinput1_4 xinput9_1_0)

file(GLOB _xinputdll_headers CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/library/wine/xinputshared.h)
set(_xinputdll_source ${CMAKE_CURRENT_SOURCE_DIR}/xinput.c)

# Put them next to the launcher and libtas.so, like when installed
set(_xinputdll_dir ${PROJECT_BINARY_DIR}/src/program)

# The loader of Wine only takes a dll as one of its own if the dos header
# starts with this signature (as winebuild writes it)
set(_xinputdll_signature "Wine builtin DLL\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0")

set(_xinputdll_outputs)
foreach(_arch 32 64)
    if(_arch EQUAL 32)
        set(_cc ${LIBTAS_MINGW32_CC})
        set(_suffix "")
    else()
        set(_cc ${LIBTAS_MINGW64_CC})
        set(_suffix "_64")
    endif()

    if(NOT _cc)
        message(STATUS "mingw-w64 compiler for ${_arch}-bit Windows not found, controllers will not work for ${_arch}-bit Wine games")
        continue()
    endif()

    foreach(_version ${_xinput_versions})
        set(_dll libtas-${_version}${_suffix}.dll)
        add_custom_command(
            OUTPUT ${_xinputdll_dir}/${_dll}
            COMMAND ${_cc} -O2 -shared -static-libgcc
                    -I${PROJECT_SOURCE_DIR}/src/library
                    ${_xinputdll_source} ${CMAKE_CURRENT_SOURCE_DIR}/${_version}.def
                    -Wl,--kill-at,--enable-stdcall-fixup -o ${_xinputdll_dir}/${_dll}
            COMMAND sh -c "printf '${_xinputdll_signature}' | dd of='${_xinputdll_dir}/${_dll}' bs=1 seek=64 conv=notrunc 2>/dev/null"
            DEPENDS ${_xinputdll_source} ${CMAKE_CURRENT_SOURCE_DIR}/${_version}.def ${_xinputdll_headers}
            COMMENT "Building ${_dll}"
            VERBATIM)
        list(APPEND _xinputdll_outputs ${_xinputdll_dir}/${_dll})
    endforeach()
endforeach()

if(_xinputdll_outputs)
    add_custom_target(xinputdll ALL DEPENDS ${_xinputdll_outputs})
    install(FILES ${_xinputdll_outputs} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
