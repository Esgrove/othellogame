#!/bin/bash
set -eo pipefail

USAGE="Usage: $(basename "$0") [OPTIONS]

Build Othello C# binary.

OPTIONS: All options are optional
    -h | --help                 Display these instructions.
    -b | --build-type <type>    Specify build type. Default is 'Release'.
    -t | --test                 Run tests.
    -v | --verbose              Display commands being executed.
"

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

init_options() {
    BUILD_TYPE="Release"
    RUN_TESTS=false

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
            -t | --test)
                RUN_TESTS=true
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

run_tests() {
    cd "$PROJECT_PATH"
    print_magenta "Running tests..."
    dotnet test --verbosity normal
}

init_options "$@"
build_project

if [ "$RUN_TESTS" = true ]; then
    run_tests
fi
