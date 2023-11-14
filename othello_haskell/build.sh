#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/othello_haskell"

print_magenta "Building Othello Haskell..."

if [ -z "$(command -v cabal)" ]; then
    print_error_and_exit "cabal not found in path"
else
    cabal --version
fi

pushd "$PROJECT_PATH" > /dev/null

if [ "$PLATFORM" = windows ]; then
    executable="othello_haskell.exe"
else
    executable="othello_haskell"
fi

cabal build

cp "$(find dist-newstyle/build -type f -name "othello_haskell" -print -quit)" "$executable"
file "$executable"
./"$executable" --version
./"$executable" -h
popd > /dev/null
