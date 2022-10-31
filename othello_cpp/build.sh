#!/bin/bash
set -e

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
CMAKE_BUILD_DIR="$PROJECT_PATH/cmake-build-$PLATFORM"

BUILD_TYPE="Release"

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

generate_windows_project() {
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
    # TODO: fix ccache invocation on Windows
    ccache_zero_stats
    if [ "$PLATFORM" = windows ]; then
        if ! generate_windows_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_windows_project
        fi
        time cmake --build "$CMAKE_BUILD_DIR" --target othello_cpp --config "$BUILD_TYPE"
    else
        if ! generate_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_project
        fi
        time ninja -C "$CMAKE_BUILD_DIR" othello_cpp
    fi
    ccache_show_stats
    echo "Build succeeded"
}

build_project

if [ "$PLATFORM" = windows ]; then
    # Move executable from build dir to project root
    mv "$CMAKE_BUILD_DIR/Release/othello_cpp.exe" othello_cpp.exe
    # Run executable to check it works and print the version info
    ./othello_cpp.exe --version
else
    mv "$CMAKE_BUILD_DIR/othello_cpp" othello_cpp
    ./othello_cpp --version
fi
