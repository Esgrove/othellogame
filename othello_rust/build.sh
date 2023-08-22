#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/othello_rust"

if [ -z "$(command -v cargo)" ]; then
    echo "Cargo not found in path. Maybe install rustup?"
    exit 1
fi

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
