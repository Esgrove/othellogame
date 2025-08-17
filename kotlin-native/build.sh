#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

print_magenta "Building Othello Kotlin..."

rm -rf build/bin/native/releaseExecutable/*.kexe

./gradlew nativeBinaries

executable="othello_kotlin.kexe"
rm -f "$executable"
mv build/bin/native/releaseExecutable/othello_kotlin.kexe "$executable"
file "$executable"
./"$executable" --version
./"$executable" -h || :
