#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

print_magenta "Building Othello Java..."

rm -rf build/libs/othello_java-*-all.jar

./gradlew fatJar

jar="$(find build -iname "othello_java-*-all.jar" -print -quit)"
executable="othello_java.jar"

rm -f "$executable"
mv "$jar" "$executable"
file "$executable"
print_magenta "Version:"
java -jar "$executable" --version
print_magenta "Usage:"
java -jar "$executable" -h || :
