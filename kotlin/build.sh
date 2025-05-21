#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

print_magenta "Building Othello Kotlin..."

rm -rf build/libs/othello_kotlin*.jar

./gradlew shadowJar

jar="$(find build -iname "othello_kotlin-*-all.jar" -print -quit)"
executable="othello_kotlin.jar"

rm -f "$executable"
mv "$jar" "$executable"
file "$executable"
java -jar "$executable" --version
java -jar "$executable" -h || :
