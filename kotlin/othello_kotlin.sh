#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

executable="othello_kotlin.jar"

if [ ! -e "$executable" ]; then
    rm -rf build/libs/*.jar
    ./gradlew --console plain --quiet shadowJar
    jar=$(find build -iname "app-*-all.jar" -print -quit)
    mv "$jar" "$executable"
fi

# Pass arguments to program
java -jar "$DIR/$executable" "$@"
