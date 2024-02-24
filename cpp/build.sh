#!/bin/bash
set -eo pipefail

USAGE="Usage: $(basename "$0") [OPTIONS]

OPTIONS: All options are optional
    -h | --help
        Display these instructions.

    -b | --build-type <type>
        Specify build type for CMake. Default is 'Release'.

    -c | --clean
        Clean temporary files before building.

    -n | --ninja
        Use Ninja generator also on Windows.

    -v | --verbose
        Display commands being executed."

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

init_options() {
    BUILD_TYPE="Release"
    USE_NINJA_ON_WINDOWS=false
    CLEAN=false

    while [ $# -gt 0 ]; do
        case "$1" in
            -h | --help)
                echo "$USAGE"
                exit 1
                ;;
            -b | --build-type)
                BUILD_TYPE="$2"
                shift
                ;;
            -c | --clean)
                CLEAN=true
                print_magenta "Cleaning..."
                git -C "$DIR" clean -fdx
                ;;
            -n | --ninja)
                USE_NINJA_ON_WINDOWS=true
                ;;
            -v | --verbose)
                set -x
                ;;
        esac
        shift
    done

    PROJECT_PATH="$REPO_ROOT/cpp"
    CMAKE_BUILD_DIR="$PROJECT_PATH/cmake-build-$BASH_PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
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
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_TESTS=OFF
}

generate_ninja_project() {
    # Use brew Clang which is typically more recent than Xcode's
    if [ "$BASH_PLATFORM" = "mac" ] && brew ls --versions llvm > /dev/null; then
        CC="$(brew --prefix llvm)/bin/clang"
        CXX="$(brew --prefix llvm)/bin/clang++"
        export CC
        export CXX
    fi
    cmake -B "$CMAKE_BUILD_DIR" \
        -G Ninja \
        -S "$PROJECT_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_TESTS=OFF
}

build_project() {
    cd "$PROJECT_PATH"
    if [ "$CLEAN" = true ]; then
        rm -rf "$CMAKE_BUILD_DIR"
    fi
    mkdir -p "$CMAKE_BUILD_DIR"

    export CCACHE_DIR="$CMAKE_BUILD_DIR/ccache"

    print_magenta "Building Othello C++..."

    if [ "$BASH_PLATFORM" = windows ] && [ "$USE_NINJA_ON_WINDOWS" != true ]; then
        echo "Generating Visual Studio project..."
        if ! generate_msvc_project; then
            print_yellow "CMake failed, removing existing cache and trying again..."
            rm -rf "$CMAKE_BUILD_DIR"
            generate_msvc_project
        fi
        print_magenta "Building $BUILD_TYPE"
        time cmake --build "$CMAKE_BUILD_DIR" --target othello_cpp --config "$BUILD_TYPE"
    else
        echo "Generating Ninja build..."
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
    print_green "Build successful"
}

move_exe_to_root() {
    cd "$PROJECT_PATH"
    local executable="othello_cpp"
    if [ "$BASH_PLATFORM" = windows ]; then
        executable="othello_cpp.exe"
    fi
    # Move executable from build dir to project root
    find "$CMAKE_BUILD_DIR" -type f -name "$executable" | head -n 1 | xargs -I {} mv {} "$executable"
    file "$executable"
    # Run executable to check it works and print the version info
    ./"$executable" --version
    ./"$executable" -h || :
}

init_options "$@"
build_project
move_exe_to_root
