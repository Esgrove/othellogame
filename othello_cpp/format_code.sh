#!/bin/bash
# Run Clang-Format for all source files under src
REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
clang-format -i -style=file "$REPO"/src/*.cpp "$REPO"/src/*.hpp
