#!/bin/bash
set -eo pipefail

# Format C++ code using ClangFormat

DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
clang-format --verbose -i -style=file:"$DIR/.clang-tidy" "$DIR"/src/*.cpp "$DIR"/src/*.hpp
