#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/typescript"
VERSION_FILE="$PROJECT_PATH/src/version.ts"

# Read project version number from package.json
get_package_version_number() {
    bun --print "require('./package.json').version"
}

update_version_information() {
    set_version_info
    VERSION_NUMBER="$(get_package_version_number)"
    VERSION_STRING="Othello TypeScript $VERSION_NUMBER $BUILD_TIME $GIT_BRANCH $GIT_COMMIT"
    {
        echo "// Updated by build script, do not edit manually!"
        echo "export const BUILD_TIME = '$BUILD_TIME';"
        echo "export const GIT_BRANCH = '$GIT_BRANCH';"
        echo "export const GIT_COMMIT = '$GIT_COMMIT';"
        echo "export const VERSION_NUMBER = '$VERSION_NUMBER';"
        echo "export const VERSION_STRING = '$VERSION_STRING';"
        echo ""
        echo "/** Return version info string. */"
        echo "export function versionInfo(): string {"
        echo "    return VERSION_STRING;"
        echo "}"
    } > "$VERSION_FILE"
}

cd "$PROJECT_PATH"

if [ -z "$(command -v bun)" ]; then
    print_error_and_exit "bun not found in path"
else
    echo "bun $(bun --version)"
fi

print_magenta "Building Othello TypeScript..."
bun install --frozen-lockfile
update_version_information
bun run build

print_magenta "Version:"
bun src/main.ts --version
print_magenta "Usage:"
bun src/main.ts -h || :
