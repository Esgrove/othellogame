#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

FORCE_BUILD=false
RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -f | --force)
            FORCE_BUILD=true
            ;;
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

if [ "$FORCE_BUILD" = true ]; then
    # Touch build script to rerun it so version info gets updated,
    # since it only reruns when build.rs itself changes (cargo:rerun-if-changed).
    touch build.rs
fi

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
