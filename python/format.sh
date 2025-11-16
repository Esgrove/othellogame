#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$REPO_ROOT/python"

if [ -z "$(command -v uv)" ]; then
    print_error_and_exit "uv not found in path"
fi

uv run ruff format
uv run ruff check --fix
