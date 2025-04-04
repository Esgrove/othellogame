#!/bin/bash
set -eo pipefail

REPO_ROOT=$(git rev-parse --show-toplevel || (cd "$(dirname "${BASH_SOURCE[0]}")" && pwd))
export REPO_ROOT

# Check platform
case "$(uname -s)" in
    "Darwin")
        BASH_PLATFORM="mac"
        ;;
    "MINGW"*)
        BASH_PLATFORM="windows"
        ;;
    "MSYS"*)
        BASH_PLATFORM="windows"
        ;;
    *)
        BASH_PLATFORM="linux"
        ;;
esac
export BASH_PLATFORM

check_and_set_python() {
    # Check Python is found on path and set PYTHON variable to it
    if [ -n "$(command -v python3)" ]; then
        PYTHON=$(which python3)
    elif [ -n "$(command -v python)" ]; then
        PYTHON=$(which python)
    else
        print_error_and_exit "Python not found in path"
    fi

    if ! $PYTHON -c 'import sys; sys.exit(1) if sys.version_info < (3, 11) else sys.exit(0)'; then
        print_error_and_exit "Python 3.11+ required"
    fi
    export PYTHON
}

# Set variables BUILD_TIME, GIT_COMMIT, and GIT_BRANCH
set_version_info() {
    BUILD_TIME=$(date -u +"%Y-%m-%d_%H%M")
    GIT_COMMIT=$(git -C "$REPO_ROOT" rev-parse --short HEAD)
    GIT_BRANCH=$(git -C "$REPO_ROOT" branch --show-current)
    export BUILD_TIME
    export GIT_COMMIT
    export GIT_BRANCH
}

# Print a message with green color
print_green() {
    printf "\e[1;49;32m%s\e[0m\n" "$1"
}

# Print a message with magenta color
print_magenta() {
    printf "\e[1;49;35m%s\e[0m\n" "$1"
}

# Print a message with red color
print_red() {
    printf "\e[1;49;31m%s\e[0m\n" "$1"
}

# Print a message with yellow color
print_yellow() {
    printf "\e[1;49;33m%s\e[0m\n" "$1"
}

print_error() {
    print_red "ERROR: $1"
}

print_warn() {
    print_yellow "WARNING: $1"
}

# Print an error and exit the program
print_error_and_exit() {
    print_error "$1"
    # use exit code if given as argument, otherwise default to 1
    exit "${2:-1}"
}

# Print usage and exit the program. An optional error message can be given as well.
print_usage_and_exit() {
    if [ $# -eq 1 ]; then
        print_error "$1"
    fi
    if [ -z "$USAGE" ]; then
        print_red "No usage text provided in variable USAGE"
    else
        echo "$USAGE"
    fi
    # use exit code if given as second argument, otherwise default to 1
    exit "${2:-1}"
}
