#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/typescript"

cd "$PROJECT_PATH"

if [ -z "$(command -v bun)" ]; then
    print_error_and_exit "bun not found in path"
fi

# Pass arguments to program
bun src/main.ts "$@"
