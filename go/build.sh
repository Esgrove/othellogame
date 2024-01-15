#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Define project version here, normally would use git tags for this...
PROJECT_VERSION="1.4.0"

USAGE="Usage: $(basename "$0") [OPTIONS]

OPTIONS: All options are optional
    -h | --help
        Display these instructions.

    -v | --verbose
        Display commands being executed."

init_options() {
    while [ $# -gt 0 ]; do
        case "$1" in
            -h | --help)
                echo "$USAGE"
                exit 1
                ;;
            -v | --verbose)
                set -x
                ;;
        esac
        shift
    done

    # Get absolute path to repo root
    REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
    PROJECT_PATH="$REPO_ROOT/go"

    if [ "$PLATFORM" = windows ]; then
        EXECUTABLE="othello_go.exe"
    else
        EXECUTABLE="othello_go"
    fi
}

build_project() {
    print_magenta "Building Othello Go..."

    if [ -z "$(command -v go)" ]; then
        print_error_and_exit "go not found in path"
    else
        go version
    fi

    pushd "$PROJECT_PATH" > /dev/null
    rm -f "$EXECUTABLE"
    time go build -v \
        -ldflags "-X othello_go/othello.GitBranch=$GIT_BRANCH \
                  -X othello_go/othello.GitHash=$GIT_HASH \
                  -X othello_go/othello.Timestamp=$BUILD_TIME \
                  -X othello_go/othello.VersionNumber=$PROJECT_VERSION"

    file "$EXECUTABLE"
    ./"$EXECUTABLE" --version
    ./"$EXECUTABLE" -h
    popd > /dev/null
}

update_version_file() {
    VERSION_FILE="$PROJECT_PATH/othello/version.go"
    {
        echo "package othello"
        echo ""
        echo "// Generated automatically; DO NOT EDIT MANUALLY."
        echo ""
        echo "const VersionNumber = \"$PROJECT_VERSION\""
        echo "const GitBranch = \"$GIT_BRANCH\""
        echo "const GitHash = \"$GIT_HASH\""
        echo "const Timestamp = \"$BUILD_TIME\""

    } > "$VERSION_FILE"
}

init_options "$@"
set_version_info
# Using variables set during compile time instead of hardcoded version file
# update_version_file
build_project
