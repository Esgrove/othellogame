#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

executable="othello_java.jar"

if [ ! -e "$executable" ]; then
    rm -rf build/libs/othello_java*.jar
    ./gradlew --console plain --quiet fatJar
    jar=$(find build/libs -iname "othello_java-*-all.jar" -print -quit)
    mv "$jar" "$executable"
fi

# Run the JAR with any provided arguments
java -jar "$DIR/$executable" "$@"
