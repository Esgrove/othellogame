#!/bin/bash
set -eo pipefail

# Format C++ code using ClangFormat

REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
clang-format --verbose -i -style=file "$REPO"/src/*.cpp "$REPO"/src/*.hpp
