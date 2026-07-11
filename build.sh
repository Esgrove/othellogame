#!/bin/bash
set -eo pipefail

USAGE="Usage: $(basename "$0") [OPTIONS]

Build all Othello implementations.

OPTIONS: All options are optional
    -h | --help       Display these instructions.
    -f | --force      Force rebuild so version info gets updated.
    -t | --test       Run tests.
    -v | --verbose    Display commands being executed.
"

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

init_options() {
    BUILD_FLAGS=()

    while [ $# -gt 0 ]; do
        case "$1" in
            -h | --help)
                echo "$USAGE"
                exit 1
                ;;
            -f | --force)
                BUILD_FLAGS+=("--force")
                ;;
            -t | --test)
                BUILD_FLAGS+=("--test")
                ;;
            -v | --verbose)
                set -x
                ;;
            *)
                print_warn "Unknown argument '$1'"
                print_usage_and_exit
                ;;
        esac
        shift
    done
}

# Run the build script of every implementation, passing the build flags.
build_all() {
    find . -mindepth 2 -maxdepth 2 -name "build.sh" -print -exec bash {} "${BUILD_FLAGS[@]}" \;
}

# Print the version info for every built executable.
check_versions() {
    echo ""
    print_magenta "Checking versions:"
    if [ "$BASH_PLATFORM" = windows ]; then
        ./cpp/othello_cpp.exe --version
        ./csharp/othello_csharp.exe --version
        ./go/othello_go.exe --version
        ./kotlin-native/othello_kotlin.exe --version
        ./rust/othello_rust.exe --version
        #./swift/othello_swift.exe --version
    else
        ./cpp/othello_cpp --version
        ./csharp/othello_csharp --version
        ./go/othello_go --version
        ./kotlin-native/othello_kotlin --version
        ./rust/othello_rust --version
        ./swift/othello_swift --version
    fi

    java -jar java/othello_java.jar --version
    java -jar kotlin/othello_kotlin.jar --version
    ./python/othello_python.sh --version
    ./typescript/othello_typescript.sh --version
}

init_options "$@"
cd "$DIR"
build_all
check_versions
