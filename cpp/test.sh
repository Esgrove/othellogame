#!/bin/bash
set -eo pipefail

USAGE="Usage: $(basename "$0") [OPTIONS]

Build and run tests for Othello C++

OPTIONS: All options are optional
    -h | --help
        Display these instructions.

    -b | --build-type <type>
        Specify build type for CMake. Default is 'Release'.

    -c | --clean
        Clean temporary files before building.

    -v | --verbose
        Display commands being executed."

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

init_options() {
    BUILD_TYPE="Release"
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
            -v | --verbose)
                set -x
                ;;
        esac
        shift
    done

    PROJECT_PATH="$REPO_ROOT/cpp"
    CMAKE_BUILD_DIR="$PROJECT_PATH/cmake-build-$BASH_PLATFORM-ninja-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
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
        -DBUILD_TESTS=ON
}

build_project() {
    cd "$PROJECT_PATH"
    if [ "$CLEAN" = true ]; then
        rm -rf "$CMAKE_BUILD_DIR"
    fi
    mkdir -p "$CMAKE_BUILD_DIR"

    export CCACHE_DIR="$CMAKE_BUILD_DIR/ccache"

    print_magenta "Building Othello C++ tests..."

    echo "Generating Ninja build..."
    if ! generate_ninja_project; then
        print_yellow "CMake failed, removing existing cache and trying again..."
        rm -rf "$CMAKE_BUILD_DIR"
        generate_ninja_project
    fi
    print_magenta "Building $BUILD_TYPE with tests"
    time ninja -C "$CMAKE_BUILD_DIR" othello_tests
    print_green "Build successful"
}

run_tests() {
    cd "$CMAKE_BUILD_DIR"
    print_magenta "Running tests..."
    ctest --output-on-failure
    print_green "Tests successful"
}

init_options "$@"
build_project
run_tests
