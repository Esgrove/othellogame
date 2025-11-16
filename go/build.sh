#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Define project version here, normally would use git tags for this...
PROJECT_VERSION="1.9.0"

USAGE="Usage: $(basename "$0") [OPTIONS]

Build Othello Go binary.

OPTIONS: All options are optional
    -h | --help       Display these instructions.
    -v | --verbose    Display commands being executed.
"

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
            *)
                print_warn "Unknown argument '$1'"
                print_usage_and_exit
                ;;
        esac
        shift
    done

    # Get absolute path to repo root
    REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
    PROJECT_PATH="$REPO_ROOT/go"

    EXECUTABLE="othello_go"
    if [ "$BASH_PLATFORM" = windows ]; then
        EXECUTABLE+=".exe"
    fi
}

build_project() {
    print_magenta "Building Othello Go..."

    if [ -z "$(command -v go)" ]; then
        print_error_and_exit "go not found in path"
    else
        go version
    fi

    echo "BUILD_TIME: $BUILD_TIME"
    echo "GIT_COMMIT: $GIT_COMMIT"
    echo "GIT_BRANCH: $GIT_BRANCH"

    VERSION_INFO="-X othello_go/othello.GitBranch=$GIT_BRANCH \
                  -X othello_go/othello.GitCommit=$GIT_COMMIT \
                  -X othello_go/othello.BuildTime=$BUILD_TIME \
                  -X othello_go/othello.VersionNumber=$PROJECT_VERSION"

    cd "$PROJECT_PATH"
    rm -f "$EXECUTABLE"
    time go build -v -ldflags "$VERSION_INFO"

    file "$EXECUTABLE"
    ./"$EXECUTABLE" --version
    ./"$EXECUTABLE" -h || :
}

init_options "$@"
set_version_info
build_project
