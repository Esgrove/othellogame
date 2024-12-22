#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

executable="othello_kotlin.kexe"

if [ ! -e "$executable" ]; then
    rm -rf build/bin/native/releaseExecutable/*.kexe
    ./gradlew --console plain --quiet nativeBinaries
    mv build/bin/native/releaseExecutable/othello_kotlin.kexe "$executable"
fi

# Pass arguments to program
./"$executable" "$@"
