#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -t | --test)
            RUN_TESTS=true
            ;;
    esac
done

print_magenta "Building Othello Kotlin..."

# Touch main file to trigger rebuild so version info gets updated
touch src/nativeMain/kotlin/othello/Main.kt

rm -rf build/bin/native/releaseExecutable/*.kexe

./gradlew nativeBinaries

# Drop ".kexe" extension from binary
executable="othello_kotlin"
if [ "$BASH_PLATFORM" = windows ]; then
    executable+=".exe"
    output="build/bin/native/releaseExecutable/othello_kotlin.exe"
else
    output="build/bin/native/releaseExecutable/othello_kotlin.kexe"
fi

rm -f "$executable"
mv "$output" "$executable"
file "$executable"
print_magenta "Version:"
./"$executable" --version
print_magenta "Usage:"
./"$executable" -h || :

if [ "$RUN_TESTS" = true ]; then
    print_magenta "Running tests..."
    ./gradlew allTests
fi
