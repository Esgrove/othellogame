#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

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
    PROJECT_PATH="$REPO_ROOT/othello_go"

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
    time go build -race -v

    file "$EXECUTABLE"
    ./"$EXECUTABLE" --version
    popd > /dev/null
}

init_options "$@"
build_project
