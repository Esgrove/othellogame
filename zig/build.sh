#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

print_magenta "Building Othello Zig..."

if [ -z "$(command -v zig)" ]; then
    print_error_and_exit "Zig not found in path"
else
    echo "Zig $(zig version) from $(which zig)"
fi

cd "$REPO_ROOT/zig"

VERSION=$(grep '.version' "build.zig.zon" | awk -F'"' '{ print $2 }')

# Path to the version.zig file
VERSION_FILE="src/version.zig"

# Write the version constant to version.zig
echo "pub const version = \"${VERSION}\";" > "${VERSION_FILE}"

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
