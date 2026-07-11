#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/python"

cd "$PROJECT_PATH"

if [ -z "$(command -v uv)" ]; then
    print_error_and_exit "uv not found in path"
fi

# Pass arguments to program
uv run --quiet othello/othello.py "$@"
