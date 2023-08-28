#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/othello_go"

print_magenta "Building Othello Go..."

if [ -z "$(command -v go)" ]; then
    print_error_and_exit "go not found in path"
else
    go version
fi

pushd "$PROJECT_PATH" > /dev/null
time go build -race -v

if [ "$PLATFORM" = windows ]; then
    executable="othello_go.exe"
else
    executable="othello_go"
fi

file "$executable"
./"$executable" --version
popd > /dev/null
