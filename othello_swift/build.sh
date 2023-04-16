#!/bin/bash
set -eo pipefail

# Get absolute path to repo root
REPO_PATH=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_PATH/othello_swift"

if [ -z "$(command -v swift)" ]; then
    echo "Error: swift not found in path"
    exit 1
fi

# Check platform
case "$(uname -s)" in
    "Darwin")
        PLATFORM="mac"
        ;;
    "MINGW"*)
        PLATFORM="windows"
        ;;
    *)
        PLATFORM="linux"
        ;;
esac

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
