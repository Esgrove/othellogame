#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

cd "$DIR"

rm -rf build/libs/othello_java*.jar

./gradlew --console plain --quiet fatJar

# Find the generated JAR
FAT_JAR=$(find build/libs -iname "othello_java-*-all.jar" | head -n 1)

mv "$FAT_JAR" ./othello_java.jar

# Run the JAR with any provided arguments
java -jar ./othello_java.jar "$@"
