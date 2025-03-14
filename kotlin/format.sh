#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

if [ -z "$(command -v ktlint)" ]; then
    print_error_and_exit "ktlint not found"
fi

cd "$DIR"
ktlint --format --relative
