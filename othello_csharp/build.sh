#!/bin/bash
set -e

USAGE="Usage: build.sh [OPTIONS]

OPTIONS: All options are optional
    --help
        Display these instructions.

    --build-type <type>
        Specify build type. Default is 'Release'.

    --verbose
        Display commands being executed.
"

init_options() {
    BUILD_TYPE="Release"

    while [ $# -gt 0 ]; do
        case "$1" in
            --help)
                echo "$USAGE"
                exit 1
                ;;
            --build-type)
                BUILD_TYPE="$2"
                shift
                ;;
            --verbose)
                set -x
                ;;
        esac
        shift
    done

    # Check platform
    case "$(uname -s)" in
        "Darwin")
            PLATFORM="mac"
            RUNTIME="osx.10.13-x64"
            # TODO: osx.11.0-arm64
            ;;
        "MINGW"*)
            PLATFORM="windows"
            RUNTIME="win10-x64"
            ;;
        *)
            PLATFORM="linux"
            RUNTIME="linux-x64"
            ;;
    esac

    # Get absolute path to repo root
    REPO_PATH=$(git rev-parse --show-toplevel || (cd "$(dirname "../${BASH_SOURCE[0]}")" && pwd))
    PROJECT_PATH="$REPO_PATH/othello_csharp"
    BUILD_DIR="$PROJECT_PATH/dotnet-build-$PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
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

# Print an error and exit the program
print_error_and_exit() {
    print_red "ERROR: $1"
    # use exit code if given as argument, otherwise default to 1
    exit "${2:-1}"
}

build_project() {
    cd "$PROJECT_PATH"
    mkdir -p "$BUILD_DIR"

    if [ -z "$(command -v dotnet)" ]; then
        print_error_and_exit "dotnet not found in path"
    fi

    print_magenta "Building $BUILD_TYPE"
    dotnet publish --configuration "$BUILD_TYPE" --self-contained true --runtime "$RUNTIME" --output "$BUILD_DIR"

    # Move executable from build dir to project root
    if [ "$PLATFORM" = windows ] && [ "$USE_NINJA_ON_WINDOWS" != true ]; then
        mv "$(find "$BUILD_DIR" -name Othello.exe)" othello_csharp.exe
        file othello_csharp.exe
    else
        mv "$(find "$BUILD_DIR" -name Othello)" othello_csharp
        file othello_csharp
    fi
}

init_options "$@"
build_project
