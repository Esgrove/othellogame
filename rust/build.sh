#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

print_magenta "Building Othello Rust..."

if [ -z "$(command -v cargo)" ]; then
    print_error_and_exit "Cargo not found in path"
else
    echo "$(rustc --version) from $(which rustc)"
fi

cd "$REPO_ROOT/rust"

cargo build --release

if [ "$BASH_PLATFORM" = windows ]; then
    executable="othello_rust.exe"
else
    executable="othello_rust"
fi

rm -f "$executable"
mv ./target/release/"$executable" "$executable"
file "$executable"
./"$executable" --version
./"$executable" -h || :
