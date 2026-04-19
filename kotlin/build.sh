#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

RUN_TESTS=false
for arg in "$@"; do
    case "$arg" in
        -t | --test)
            RUN_TESTS=true
            ;;
    esac
done

print_magenta "Building Othello Kotlin..."

# Touch main file to trigger rebuild so version info gets updated
touch src/main/kotlin/othello/Main.kt

rm -rf build/libs/othello_kotlin*.jar

./gradlew shadowJar

jar="$(find build -iname "othello_kotlin-*-all.jar" -print -quit)"
executable="othello_kotlin.jar"

rm -f "$executable"
mv "$jar" "$executable"
file "$executable"
print_magenta "Version:"
java -jar "$executable" --version
print_magenta "Usage:"
java -jar "$executable" -h || :

if [ "$RUN_TESTS" = true ]; then
    print_magenta "Running tests..."
    ./gradlew test
fi
