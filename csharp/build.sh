#!/bin/bash
set -eo pipefail

USAGE="Usage: $(basename "$0") [OPTIONS]

Build Othello C# binary.

OPTIONS: All options are optional
    -h | --help                 Display these instructions.
    -b | --build-type <type>    Specify build type. Default is 'Release'.
    -v | --verbose              Display commands being executed.
"

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

init_options() {
    BUILD_TYPE="Release"

    while [ $# -gt 0 ]; do
        case "$1" in
            -h | --help)
                echo "$USAGE"
                exit 1
                ;;
            -b | --build-type)
                BUILD_TYPE="$2"
                shift
                ;;
            -v | --verbose)
                set -x
                ;;
            *)
                print_warn "Unknown argument '$1'"
                print_usage_and_exit
                ;;
        esac
        shift
    done

    EXECUTABLE="othello_csharp"
    if [ "$BASH_PLATFORM" = mac ]; then
        if [ "$(uname -m)" = arm64 ]; then
            RUNTIME="osx-arm64"
        else
            RUNTIME="osx-x64"
        fi
    elif [ "$BASH_PLATFORM" = windows ]; then
        RUNTIME="win-x64"
        EXECUTABLE+=".exe"
    else
        RUNTIME="linux-x64"
    fi

    PROJECT_PATH="$REPO_ROOT/csharp"
    BUILD_DIR="$PROJECT_PATH/dotnet-build-$BASH_PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
    VERSION_FILE="$PROJECT_PATH/Othello/Version.cs"
}

update_version_info() {
    set_version_info
    VERSION="$(awk -F'[<>]' '/<Version>/{print $3}' "$PROJECT_PATH/Othello/Othello.csproj")"
    print_yellow "Writing version information with version $VERSION"
    echo "BUILD_TIME: $BUILD_TIME"
    echo "GIT_COMMIT: $GIT_COMMIT"
    echo "GIT_BRANCH: $GIT_BRANCH"
    {
        echo "// Generated automatically by build script; DO NOT EDIT MANUALLY."
        echo ""
        echo "namespace Othello {"
        echo "    public static class Version {"
        echo "        public const string BuildTime = \"$BUILD_TIME\";"
        echo "        public const string GitBranch = \"$GIT_BRANCH\";"
        echo "        public const string GitCommit = \"$GIT_COMMIT\";"
        echo "        public const string VersionNumber = \"$VERSION\";"
        echo "    }"
        echo "}"
    } > "$VERSION_FILE"
}

build_project() {
    cd "$PROJECT_PATH"
    mkdir -p "$BUILD_DIR"

    print_magenta "Building Othello C#..."

    if [ -z "$(command -v dotnet)" ]; then
        print_error_and_exit "dotnet not found in path"
    else
        echo "dotnet $(dotnet --version)"
    fi

    echo "Building Othello C# $BUILD_TYPE for $RUNTIME"
    time dotnet publish \
        Othello/Othello.csproj \
        --configuration "$BUILD_TYPE" \
        --property:PublishDir="$BUILD_DIR" \
        --runtime "$RUNTIME" \
        --self-contained true \
        --verbosity minimal

    # Move executable from build dir to project root
    if [ "$BASH_PLATFORM" = windows ]; then
        mv "$(find "$BUILD_DIR" -type f -name Othello.exe)" $EXECUTABLE
    else
        mv "$(find "$BUILD_DIR" -type f -name Othello)" $EXECUTABLE
    fi
    file "$EXECUTABLE"
    print_magenta "Version:"
    ./"$EXECUTABLE" --version
    print_magenta "Usage:"
    ./"$EXECUTABLE" -h || :
}

init_options "$@"
update_version_info
build_project
