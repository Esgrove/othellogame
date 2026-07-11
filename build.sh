#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

cd "$DIR"

FORCE_BUILD=false
RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -f | --force)
            FORCE_BUILD=true
            ;;
        -t | --test)
            RUN_TESTS=true
            ;;
    esac
done

BUILD_FLAGS=()
if [ "$FORCE_BUILD" = true ]; then
    BUILD_FLAGS+=("--force")
fi
if [ "$RUN_TESTS" = true ]; then
    BUILD_FLAGS+=("--test")
fi

find . -mindepth 2 -maxdepth 2 -name "build.sh" -print -exec bash {} "${BUILD_FLAGS[@]}" \;

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
