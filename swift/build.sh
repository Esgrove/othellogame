#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/swift"

VERSION_NUMBER=$(cat "$PROJECT_PATH/VERSION")

RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -t | --test)
            RUN_TESTS=true
            ;;
    esac
done

print_magenta "Building Othello Swift..."
echo "Version: $VERSION_NUMBER"

if [ -z "$(command -v swift)" ]; then
    print_error_and_exit "swift not found in path"
else
    swift --version
fi

cd "$PROJECT_PATH"

# Touch main source file to trigger rebuild so version info gets updated
touch Sources/Othello/main.swift

swift build --configuration release

executable="othello_swift"
if [ "$BASH_PLATFORM" = windows ]; then
    executable+=".exe"
fi

rm -f "$executable"
executable_path="$(find .build -maxdepth 2 -mindepth 2 -type d -path ".build/*/release" -print -quit)/$executable"
mv "$executable_path" "$executable"
file "$executable"
print_magenta "Version:"
./"$executable" --version
print_magenta "Usage:"
./"$executable" -h || :

if [ "$RUN_TESTS" = true ]; then
    cd "$PROJECT_PATH"
    print_magenta "Running tests..."
    swift test
fi
