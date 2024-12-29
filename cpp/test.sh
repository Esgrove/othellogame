#!/bin/bash
set -eo pipefail

# Build and run tests for Othello C++

cd "$(dirname "${BASH_SOURCE[0]}")"

./build.sh --test "$@"
