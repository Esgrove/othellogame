#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/othello_swift"
VERSION_HEADER="$PROJECT_PATH/Sources/BuildInfo/include/version.h"

# Version info
BUILD_TIME=$(date +"%Y-%m-%d_%H%M")
GIT_HASH=$(git rev-parse --short HEAD)
GIT_BRANCH=$(git branch --show-current)

echo "#define BUILD_TIME \"${BUILD_TIME}\"" > "$VERSION_HEADER"
echo "#define GIT_HASH \"${GIT_HASH}\"" >> "$VERSION_HEADER"
echo "#define GIT_BRANCH \"${GIT_BRANCH}\"" >> "$VERSION_HEADER"

print_magenta "Building Othello Swift..."

if [ -z "$(command -v swift)" ]; then
    print_error_and_exit "swift not found in path"
else
    swift --version
fi

pushd "$PROJECT_PATH" > /dev/null
swift build --configuration release

if [ "$PLATFORM" = windows ]; then
    executable="othello_swift.exe"
else
    executable="othello_swift"
fi

rm -f "$executable"
mv "$(find .build -type d -name release -print -quit)/$executable" "$executable"
file "$executable"
./"$executable" --version
popd > /dev/null
