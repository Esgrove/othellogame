#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

VERSION_FILE="$DIR/othello/version.go"

CURRENT_VERSION="$(grep "const VersionNumber =" "$VERSION_FILE" | cut -d\" -f 2)"
MAJOR=$(echo "$CURRENT_VERSION" | cut -d '.' -f 1)
MINOR=$(echo "$CURRENT_VERSION" | cut -d '.' -f 2)

echo "Current version:    $CURRENT_VERSION"

if [ -z "$VERSION" ]; then
    MINOR=$((MINOR + 1))
    VERSION="$MAJOR.$MINOR.0"
fi

if [[ "$VERSION" =~ ^[0-9]+(\.[0-9]+){2}$ ]]; then
    print_green "New version number: $VERSION"
else
    print_error_and_exit "Version number needs to be in format 'X.X.X', got: $VERSION"
fi

{
    echo "package othello"
    echo ""
    echo "// Generated automatically by go generate; DO NOT EDIT MANUALLY."
    echo ""
    echo "const VersionNumber = \"$VERSION\""
} > "$VERSION_FILE"
