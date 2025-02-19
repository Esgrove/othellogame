# Othello C++
# Akseli Lukkarila
# 2019-2025
cmake_minimum_required(VERSION 3.18)

project("Othello C++"
    VERSION 2.9.4
    DESCRIPTION "A simple Othello CLI game implementation in C++"
    HOMEPAGE_URL "https://github.com/Esgrove/othellogame"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
    # Create universal binary.
    # Disabled due to OpenSSL being arch specific by default...
    # set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif()

# Export compile commands for clang-tidy
# Note: only works with makefile and ninja generators
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build the tests" ON)

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

set(APP_BUILD_NAME "othello_cpp_${CMAKE_PROJECT_VERSION}_${GIT_HASH}_${TIMESTAMP}")

message(STATUS "Build name: ${APP_BUILD_NAME}")

if(WIN32)
    # Set OpenSSL search path for Windows
    execute_process(
        COMMAND powershell.exe -Command "[System.Environment]::GetEnvironmentVariable('USERPROFILE')"
        OUTPUT_VARIABLE USERPROFILE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(SCOOP_OPENSSL_PATH "${USERPROFILE}/scoop/apps/openssl/current")
    if(EXISTS "${SCOOP_OPENSSL_PATH}")
        set(OPENSSL_ROOT_DIR "${SCOOP_OPENSSL_PATH}")
        set(OPENSSL_INCLUDE_DIR "${SCOOP_OPENSSL_PATH}/include")
        message(STATUS "Found OpenSSL installed via Scoop")
    else()
        set(OPENSSL_ROOT_DIR "C:\\Program Files\\OpenSSL\\")
        set(OPENSSL_INCLUDE_DIR "C:\\Program Files\\OpenSSL\\include")
        message(STATUS "Using default OpenSSL path: ${OPENSSL_ROOT_DIR}")
    endif()
endif()

# OpenSSL
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    message(STATUS "OpenSSL found: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "Using OpenSSL version: ${OPENSSL_VERSION}")
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()

# ccache
# https://ccache.dev/
find_program(CCACHE_EXECUTABLE ccache)
if(CCACHE_EXECUTABLE)
    message(STATUS "ccache found: ${CCACHE_EXECUTABLE}")
    # print ccache version with nice formatting
    execute_process(
        COMMAND ${CCACHE_EXECUTABLE} --version
        OUTPUT_VARIABLE CCACHE_VERSION_INFO
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "^[^\n]*" CCACHE_VERSION "${CCACHE_VERSION_INFO}")
    message(STATUS "${CCACHE_VERSION}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
endif(CCACHE_EXECUTABLE)

# {fmt} is an open-source formatting library providing a fast and safe alternative to C stdio and C++ iostreams.
# https://github.com/fmtlib/fmt
# This can be used if fmt is installed locally (brew, vcpkg, apt...)
# find_package(fmt 9 REQUIRED)
message(STATUS "Fetching fmt library")
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 11.1.3
)
FetchContent_MakeAvailable(fmt)

# CLI argument parsing library
# https://github.com/jarro2783/cxxopts
message(STATUS "Fetching cxxopts library")
FetchContent_Declare(cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts
  GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(cxxopts)

# Set version information
set(VERSION_INFO_DEFINITIONS
    BUILDTIME_APP_NAME="${CMAKE_PROJECT_NAME}"
    BUILDTIME_BRANCH="${GIT_BRANCH}"
    BUILDTIME_BUILD_NAME="${APP_BUILD_NAME}"
    BUILDTIME_COMMIT="${GIT_HASH}"
    BUILDTIME_DATE="${DATE}"
    BUILDTIME_TIMESTAMP="${TIMESTAMP}"
    BUILDTIME_VERSION_NUMBER="${CMAKE_PROJECT_VERSION}"
)

set_property(GLOBAL PROPERTY GLOBAL_COMPILE_DEFINITIONS "${VERSION_INFO_DEFINITIONS}")

add_executable(othello_cpp)

target_sources(othello_cpp PRIVATE
  src/board.cpp
  src/main.cpp
  src/othello.cpp
  src/player.cpp
  src/types.cpp
  src/utils.cpp
)

target_include_directories(othello_cpp PRIVATE src)

target_compile_definitions(othello_cpp PRIVATE ${VERSION_INFO_DEFINITIONS})

target_link_libraries(othello_cpp
    cxxopts
    fmt::fmt
    OpenSSL::Crypto
)

# Enable LTO for release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    message(STATUS "Using LTO")
    set_target_properties(othello_cpp PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(STATUS "IPO is not supported: ${output}")
endif()

if(MSVC)
    # https://learn.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=msvc-170
    target_compile_options(othello_cpp PRIVATE
        /W4 /permissive-
    )
else()
    target_compile_options(othello_cpp PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_LAUNCHER: ${CMAKE_CXX_COMPILER_LAUNCHER}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
