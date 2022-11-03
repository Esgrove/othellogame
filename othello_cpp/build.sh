#!/bin/bash
set -e

USAGE="Usage: build.sh [OPTIONS]

OPTIONS: All options are optional
    --help
        Display these instructions.

    --build-type <type>
        Specify build type for CMake. Default is 'Release'.

    --ninja
        Use Ninja generator also on Windows.

    --verbose
        Display commands being executed.
"

init_options() {
    BUILD_TYPE="Release"
    USE_NINJA_ON_WINDOWS=false

    while [ $# -gt 0 ]; do
        case "$1" in
            --help)
                echo "$USAGE"
                exit 1
                ;;
            --build-type)
                BUILD_TYPE="$2"
                shift
                ;;
            --ninja)
                USE_NINJA_ON_WINDOWS=true
                ;;
            --verbose)
                set -x
                ;;
        esac
        shift
    done

    # Check platform
    case "$(uname -s)" in
        "Darwin")
            PLATFORM="mac"
            ;;
        "MINGW"*)
            PLATFORM="windows"
            ;;
        *)
            PLATFORM="linux"
            ;;
    esac

    # Get absolute path to repo root
    REPO_PATH=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
    PROJECT_PATH="$REPO_PATH/othello_cpp"
    CMAKE_BUILD_DIR="$PROJECT_PATH/cmake-build-$PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
}

# Print a message with green color
print_green() {
    printf "\e[1;49;32m%s\e[0m\n" "$1"
}

# Print a message with magenta color
print_magenta() {
    printf "\e[1;49;35m%s\e[0m\n" "$1"
}

# Print a message with red color
print_red() {
    printf "\e[1;49;31m%s\e[0m\n" "$1"
}

# Print a message with yellow color
print_yellow() {
    printf "\e[1;49;33m%s\e[0m\n" "$1"
}

# Print an error and exit the program
print_error_and_exit() {
    print_red "ERROR: $1"
    # use exit code if given as argument, otherwise default to 1
    exit "${2:-1}"
}

ccache_zero_stats() {
    if [ -n "$(command -v ccache)" ]; then
        echo "ccache: $(ccache --zero-stats)"
    else
        echo "ccache not installed"
    fi
}

ccache_show_stats() {
    if [ -n "$(command -v ccache)" ]; then
        echo "ccache:"
        ccache --show-stats
    else
        echo "ccache not installed"
    fi
}

generate_msvc_project() {
    cmake -B "$CMAKE_BUILD_DIR" \
        -G "Visual Studio 17 2022" \
        -A x64 \
        -S "$PROJECT_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
}

generate_project() {
    cmake -B "$CMAKE_BUILD_DIR" \
        -G Ninja \
        -S "$PROJECT_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
}

build_project() {
    cd "$PROJECT_PATH"
    mkdir -p "$CMAKE_BUILD_DIR"
    if [ "$PLATFORM" = windows ] && [ "$USE_NINJA_ON_WINDOWS" != true ]; then
        print_magenta "Generating Visual Studio project"
        if ! generate_msvc_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_msvc_project
        fi
        print_magenta "Building $BUILD_TYPE"
        time cmake --build "$CMAKE_BUILD_DIR" --target othello_cpp --config "$BUILD_TYPE"
    else
        print_magenta "Generating Ninja build"
        if ! generate_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_project
        fi
        print_magenta "Building $BUILD_TYPE"
        ccache_zero_stats
        time ninja -C "$CMAKE_BUILD_DIR" othello_cpp
        ccache_show_stats
    fi
    print_green "Build succeeded"
}

init_options "$@"
build_project

if [ "$PLATFORM" = windows ]; then
    # Move executable from build dir to project root
    mv "$(find " $CMAKE_BUILD_DIR" -type f -name othello_cpp.exe)" othello_cpp.exe
    file othello_cpp.exe
    # Run executable to check it works and print the version info
    ./othello_cpp.exe --version
else
    mv "$CMAKE_BUILD_DIR/othello_cpp" othello_cpp
    file othello_cpp
    ./othello_cpp --version
fi
