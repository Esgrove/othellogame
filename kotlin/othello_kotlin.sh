#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/kotlin"

cd "$PROJECT_PATH"

# Pass arguments to program
java -jar "$PROJECT_PATH/othello_kotlin.jar" "$@"
