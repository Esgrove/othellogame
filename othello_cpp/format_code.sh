#!/bin/bash
# Format C++ code using ClangFormat
set -e

REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
clang-format -i -style=file "$REPO"/src/*.cpp "$REPO"/src/*.hpp
