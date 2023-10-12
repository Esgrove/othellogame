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

# Define poject version number here since Swift Package Manager does not provide a way...
VERSION_NUMBER="2.0.0"

print_magenta "Building Othello Swift..."

echo "Writing version information..."
set_version_info
echo "#define BUILD_TIME \"${BUILD_TIME}\"" > "$VERSION_HEADER"
echo "#define GIT_HASH \"${GIT_HASH}\"" >> "$VERSION_HEADER"
echo "#define GIT_BRANCH \"${GIT_BRANCH}\"" >> "$VERSION_HEADER"
echo "#define VERSION \"${VERSION_NUMBER}\"" >> "$VERSION_HEADER"

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
