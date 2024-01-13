#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Get absolute path to repo root
REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
PROJECT_PATH="$REPO_ROOT/kotlin"

print_magenta "Building Othello Kotlin..."

if [ -z "$(command -v gradle)" ]; then
    print_error_and_exit "gradle not found in path"
else
    gradle --version
fi

pushd "$PROJECT_PATH" > /dev/null
gradle shadowJar

executable="othello_kotlin.jar"
rm -f "$executable"
mv "$(find app/build -iname "app-*-all.jar" -print -quit)" "$executable"
file "$executable"
java -jar "$executable" --version
popd > /dev/null
