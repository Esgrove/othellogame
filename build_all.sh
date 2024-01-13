#!/bin/bash
set -eo pipefail

find . -name "build.sh" -maxdepth 2 -print -exec {} \;
echo ""
./cpp/othello_cpp --version
./csharp/othello_csharp --version
./go/othello_go --version
./python/othello_python.sh --version
./rust/othello_rust --version
./swift/othello_swift --version
java -jar kotlin/othello_kotlin.jar --version
