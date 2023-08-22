#!/bin/bash
set -eo pipefail

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

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

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

    PROJECT_PATH="$REPO_ROOT/othello_cpp"
    CMAKE_BUILD_DIR="$PROJECT_PATH/cmake-build-$PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
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

generate_ninja_project() {
    cmake -B "$CMAKE_BUILD_DIR" \
        -G Ninja \
        -S "$PROJECT_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
}

build_project() {
    pushd "$PROJECT_PATH" > /dev/null
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
        if ! generate_ninja_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_ninja_project
        fi
        print_magenta "Building $BUILD_TYPE"
        ccache_zero_stats
        time ninja -C "$CMAKE_BUILD_DIR" othello_cpp
        ccache_show_stats
    fi
    print_green "Build succeeded"
    popd > /dev/null
}

move_exe_to_root() {
    pushd "$PROJECT_PATH" > /dev/null
    if [ "$PLATFORM" = windows ]; then
        executable="othello_cpp.exe"
    else
        executable="othello_cpp"
    fi
    # Move executable from build dir to project root
    mv "$(find "$CMAKE_BUILD_DIR" -type f -name "$executable")" "$executable"
    file "$executable"
    # Run executable to check it works and print the version info
    ./"$executable" --version
    popd > /dev/null
}

init_options "$@"
build_project
move_exe_to_root
