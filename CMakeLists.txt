cmake_minimum_required(VERSION 3.16)

# The version is defined once in src/shared/version.h
foreach(_part MAJOR MINOR PATCH)
    file(STRINGS src/shared/version.h _line REGEX "^static const int ${_part}VERSION = [0-9]+;")
    string(REGEX MATCH "= ([0-9]+)" _match "${_line}")
    set(_libtas_${_part} ${CMAKE_MATCH_1})
endforeach()

project(libTAS
    VERSION ${_libtas_MAJOR}.${_libtas_MINOR}.${_libtas_PATCH}
    DESCRIPTION "Tool-assisted speedrun tools for Linux games"
    HOMEPAGE_URL "https://github.com/clementgallet/libTAS"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CheckIncludeFileCXX)
include(CheckCXXSourceCompiles)
include(CMakePushCheckState)

#### Options ####

option(LIBTAS_BUILD_I386 "Build libTAS with support for 32-bit executables (also builds libtas32.so)" OFF)
option(LIBTAS_I386_LIB_ONLY "Only build the 32-bit version of the libTAS library" OFF)
option(LIBTAS_RELEASE_BUILD "Build a release (do not embed the git commit)" OFF)
option(LIBTAS_BUILD_DATE "Embed the git commit date in the executable" ON)
set(LIBTAS_QT_VERSION "" CACHE STRING "Qt major version to use (5 or 6), or empty to prefer Qt6")

if(LIBTAS_I386_LIB_ONLY)
    set(LIBTAS_BUILD_I386 ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#### 32-bit cross-compiling check ####

if(LIBTAS_BUILD_I386)
    if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$")
        message(FATAL_ERROR "Building the 32-bit library is only supported on x86_64 hosts (host is ${CMAKE_SYSTEM_PROCESSOR})")
    endif()

    cmake_push_check_state(RESET)
    set(CMAKE_REQUIRED_FLAGS -m32)
    set(CMAKE_REQUIRED_LINK_OPTIONS -m32)
    check_cxx_source_compiles("int main() { return 0; }" LIBTAS_CXX_M32)
    if(NOT LIBTAS_CXX_M32)
        message(FATAL_ERROR "Cannot build a 32-bit program, you need to install 32-bit development libraries.")
    endif()
    cmake_pop_check_state()
endif()

#### Common dependencies ####

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

#### Generated config header ####

check_include_file_cxx(xcb/randr.h LIBTAS_HAS_XCB_RANDR)

if(NOT LIBTAS_RELEASE_BUILD)
    find_package(Git QUIET)
    if(GIT_FOUND)
        # Get the latest abbreviated commit hash of the working branch
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE LIBTAS_INTERIM_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
        if(LIBTAS_INTERIM_COMMIT)
            message(STATUS "git commit hash is ${LIBTAS_INTERIM_COMMIT}")
        endif()

        if(LIBTAS_BUILD_DATE)
            execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --format=%as
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE LIBTAS_INTERIM_DATE
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
            if(LIBTAS_INTERIM_DATE)
                message(STATUS "git commit date is ${LIBTAS_INTERIM_DATE}")
            endif()
        endif()
    endif()
endif()

configure_file(cmake/config.h.cmake.in ${PROJECT_BINARY_DIR}/config.h)

#### Targets ####

add_subdirectory(src/library)

if(NOT LIBTAS_I386_LIB_ONLY)
    add_subdirectory(src/program)

    install(FILES libTAS.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install(FILES libTAS.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
endif()
