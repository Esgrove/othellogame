#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

VERSION_FILE="$DIR/othello/version.go"

if [ -z "$VERSION_NUMBER" ]; then
    VERSION_NUMBER="1.2.0"
else
    print_yellow "Using version number $VERSION_NUMBER"
fi

{
    echo "package othello"
    echo ""
    echo "// Generated automatically by go generate; DO NOT EDIT MANUALLY."
    echo ""
    echo "const VersionNumber = \"$VERSION_NUMBER\""
} > "$VERSION_FILE"
