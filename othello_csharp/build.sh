#!/bin/bash
set -eo pipefail

USAGE="Usage: build.sh [OPTIONS]

OPTIONS: All options are optional
    -h | --help
        Display these instructions.

    -b | --build-type <type>
        Specify build type. Default is 'Release'.

    -v | --verbose
        Display commands being executed.
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
        esac
        shift
    done

    # Check platform
    case "$(uname -s)" in
        "Darwin")
            PLATFORM="mac"
            if [ "$(uname -m)" = arm64 ]; then
                RUNTIME="osx.11.0-arm64"
            else
                RUNTIME="osx.10.15-x64"
            fi
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

    PROJECT_PATH="$REPO_ROOT/othello_csharp"
    BUILD_DIR="$PROJECT_PATH/dotnet-build-$PLATFORM-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"
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

    print_magenta "Building Othello C# $BUILD_TYPE for $RUNTIME"
    time dotnet publish \
        --configuration "$BUILD_TYPE" \
        --property:PublishDir="$BUILD_DIR" \
        --runtime "$RUNTIME" \
        --self-contained true \
        --verbosity minimal

    # Move executable from build dir to project root
    if [ "$PLATFORM" = windows ]; then
        mv "$(find "$BUILD_DIR" -type f -name Othello.exe)" othello_csharp.exe
        file othello_csharp.exe
    else
        mv "$(find "$BUILD_DIR" -type f -name Othello)" othello_csharp
        file othello_csharp
    fi
}

init_options "$@"
build_project
