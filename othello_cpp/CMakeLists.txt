# Othello C++
# Akseli Lukkarila
# 2019 - 2023
cmake_minimum_required(VERSION 3.18)

project("Othello C++"
    VERSION 2.1
    DESCRIPTION "Othello CLI game implementation"
    HOMEPAGE_URL "https://github.com/Esgrove/othellogame"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
# Export compile commands for clang-tidy
# Note: only works with makefile and ninja generators
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# https://cmake.org/cmake/help/latest/module/FetchContent.html
include(FetchContent)

message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "CMake source dir: ${CMAKE_SOURCE_DIR}")

# Get git hash
execute_process(
    COMMAND bash -c "git rev-parse --short HEAD"
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Get git branch
execute_process(
    COMMAND bash -c "git branch --show-current"
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Date and time
string(TIMESTAMP DATE "%Y-%m-%d" UTC)
string(TIMESTAMP TIMESTAMP "%Y-%m-%d_%H%M" UTC)

set(BUILD_NAME "othello_cpp_${CMAKE_PROJECT_VERSION}_${GIT_HASH}_${TIMESTAMP}")

message(STATUS "Build name: ${BUILD_NAME}")

# ccache
# https://ccache.dev/
find_program(CCACHE_EXECUTABLE ccache)
if(CCACHE_EXECUTABLE)
    message(STATUS "ccache found: ${CCACHE_EXECUTABLE}")
    # print version with nice formatting
    execute_process(
        COMMAND bash -c "${CCACHE_EXECUTABLE} --version | sed '1q;d'"
        OUTPUT_VARIABLE CCACHE_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "${CCACHE_VERSION}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
endif(CCACHE_EXECUTABLE)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
    # Create universal binary
    set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif()

include_directories(src)

add_executable(othello_cpp
    src/board.cpp
    src/board.hpp
    src/colorprint.hpp
    src/main.cpp
    src/othello.cpp
    src/othello.hpp
    src/player.cpp
    src/player.hpp
    src/utils.hpp
    src/version.hpp
)

# Create version info
target_compile_definitions(othello_cpp PRIVATE
    BUILDTIME_APP_NAME="${CMAKE_PROJECT_NAME}"
    BUILDTIME_BRANCH="${GIT_BRANCH}"
    BUILDTIME_BUILD_NAME="${BUILD_NAME}"
    BUILDTIME_COMMIT="${GIT_HASH}"
    BUILDTIME_DATE="${DATE}"
    BUILDTIME_VERSION_NUMBER="${CMAKE_PROJECT_VERSION}"
)

# {fmt} is an open-source formatting library providing a fast and safe alternative to C stdio and C++ iostreams.
# https://github.com/fmtlib/fmt
# This can be used if fmt is installed locally (brew, vcpkg, apt...)
# find_package(fmt 9 REQUIRED)
message(STATUS "Fetching fmt library")
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.0.0
)
FetchContent_MakeAvailable(fmt)

# https://github.com/google/googletest
message(STATUS "Fetching GoogleTest library")
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.13.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(othello_cpp
    fmt::fmt
    GTest::gtest_main
)

if (MSVC)
    # https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance
    target_compile_options(othello_cpp PRIVATE
        /W4 /permissive-
    )
else()
    target_compile_options(othello_cpp PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_LAUNCHER: ${CMAKE_CXX_COMPILER_LAUNCHER}")

message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
