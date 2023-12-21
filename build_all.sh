#!/bin/bash
set -eo pipefail

find . -name "build.sh" -maxdepth 2 -print -exec {} \;
echo ""
./othello_cpp/othello_cpp --version
./othello_csharp/othello_csharp --version
./othello_go/othello_go --version
./othello_rust/othello_rust --version
./othello_swift/othello_swift --version
./othello_python/othello_python.sh --version
java -jar othello_kotlin/othello_kotlin.jar --version
