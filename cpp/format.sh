#!/bin/bash
set -eo pipefail

# Format C++ code using ClangFormat

DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

cd "$DIR"
clang-format --verbose -i -style=file:".clang-format" "$DIR"/src/*.cpp "$DIR"/src/*.hpp
