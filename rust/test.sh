#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

print_magenta "Testing Othello Rust..."

cd "$REPO_ROOT/rust"

cargo test --no-fail-fast
