#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

cd "$DIR" > /dev/null

find . -maxdepth 2 -name "build.sh" -print -exec {} \;

echo ""
print_magenta "Checking versions:"
if [ "$BASH_PLATFORM" = windows ]; then
    ./cpp/othello_cpp.exe --version
    ./csharp/othello_csharp.exe -v
    ./go/othello_go.exe --version
    ./rust/othello_rust.exe --version
    ./swift/othello_swift.exe --version
else
    ./cpp/othello_cpp --version
    ./csharp/othello_csharp -v
    ./go/othello_go --version
    ./rust/othello_rust --version
    ./swift/othello_swift --version
fi

java -jar kotlin/othello_kotlin.jar --version
./python/othello_python.sh --version
