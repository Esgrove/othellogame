#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/swift"
VERSION_HEADER="$PROJECT_PATH/Sources/VersionInfo/versionInfo.swift"

# Define poject version number here since Swift Package Manager does not provide a way :(
VERSION_NUMBER="2.5.0"

print_magenta "Building Othello Swift..."

echo "Writing version information..."
set_version_info
echo "BUILD_TIME: $BUILD_TIME"
echo "GIT_COMMIT: $GIT_COMMIT"
echo "GIT_BRANCH: $GIT_BRANCH"
{
    echo "// Generated automatically by build script; DO NOT EDIT MANUALLY."
    echo "public enum VersionInfo {"
    echo "    public static let buildTime = \"${BUILD_TIME}\""
    echo "    public static let gitCommit = \"${GIT_COMMIT}\""
    echo "    public static let gitBranch = \"${GIT_BRANCH}\""
    echo "    public static let versionNumber = \"${VERSION_NUMBER}\""
    echo "}"
} > "$VERSION_HEADER"

if [ -z "$(command -v swift)" ]; then
    print_error_and_exit "swift not found in path"
else
    swift --version
fi

cd "$PROJECT_PATH"

swift build --configuration release

if [ "$BASH_PLATFORM" = windows ]; then
    executable="othello_swift.exe"
else
    executable="othello_swift"
fi

rm -f "$executable"
mv "$(find .build -type d -name release -print -quit)/$executable" "$executable"
file "$executable"
./"$executable" --version
./"$executable" -h || :
