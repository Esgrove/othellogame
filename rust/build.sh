#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -t | --test)
            RUN_TESTS=true
            ;;
    esac
done

print_magenta "Building Othello Rust..."

if [ -z "$(command -v cargo)" ]; then
    print_error_and_exit "Cargo not found in path"
else
    echo "$(rustc --version) from $(which rustc)"
fi

cd "$REPO_ROOT/rust"

cargo build --release

executable="othello_rust"
if [ "$BASH_PLATFORM" = windows ]; then
    executable+=".exe"
fi

rm -f "$executable"
mv ./target/release/"$executable" "$executable"
file "$executable"
print_magenta "Version:"
./"$executable" --version
print_magenta "Usage:"
./"$executable" -h || :

if [ "$RUN_TESTS" = true ]; then
    print_magenta "Running tests..."
    cargo test --no-fail-fast
fi
