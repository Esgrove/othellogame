#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/othello_swift"

if [ -z "$(command -v swift)" ]; then
    echo "Error: swift not found in path"
    exit 1
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
