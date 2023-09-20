#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

VERSION_FILE="$DIR/othello/version.go"

CURRENT_VERSION="$(grep "const VersionNumber =" "$VERSION_FILE" | cut -d\" -f 2)"
MAJOR=${CURRENT_VERSION//.*/}
MINOR=${CURRENT_VERSION##*.}

echo "Current version: $CURRENT_VERSION"

if [ -z "$VERSION" ]; then
    MINOR=$(($MINOR + 1))
    VERSION="$MAJOR.$MINOR.0"
fi

print_green "New version number: $VERSION"

{
    echo "package othello"
    echo ""
    echo "// Generated automatically by go generate; DO NOT EDIT MANUALLY."
    echo ""
    echo "const VersionNumber = \"$VERSION\""
} > "$VERSION_FILE"
