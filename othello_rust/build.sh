#!/bin/bash
set -eo pipefail

# Get absolute path to repo root
REPO_PATH=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_PATH/othello_rust"

if [ -z "$(command -v cargo)" ]; then
    echo "Cargo not found in path. Maybe install rustup?"
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
cargo build --release

if [ "$PLATFORM" = windows ]; then
    executable="othello_rust.exe"
else
    executable="othello_rust"
fi

rm -f "$executable"
mv ./target/release/"$executable" "$executable"
file "$executable"
./"$executable" --version
popd > /dev/null
