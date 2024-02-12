#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/zig"

cd "$PROJECT_PATH"

print_magenta "Building Othello Zig..."

if [ -z "$(command -v zig)" ]; then
    print_error_and_exit "Zig not found in path"
else
    echo "Zig $(zig version) from $(which zig)"
fi

VERSION_NUMBER=$(grep '.version' "$PROJECT_PATH/build.zig.zon" | awk -F'"' '{ print $2 }')
VERSION_FILE="$PROJECT_PATH/src/version.zig"

set_version_info
{
    echo "// Generated automatically by build script; DO NOT EDIT MANUALLY."
    echo "pub const BUILD_TIME = \"${BUILD_TIME}\";"
    echo "pub const GIT_HASH = \"${GIT_HASH}\";"
    echo "pub const GIT_BRANCH = \"${GIT_BRANCH}\";"
    echo "pub const VERSION = \"${VERSION_NUMBER}\";"
} > "$VERSION_FILE"

zig build -Doptimize=ReleaseFast

if [ "$PLATFORM" = windows ]; then
    executable="othello_zig.exe"
else
    executable="othello_zig"
fi

rm -f "$executable"
mv ./zig-out/bin/"$executable" "$executable"
file "$executable"
./"$executable" --version
./"$executable" -h || :
