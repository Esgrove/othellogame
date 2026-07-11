#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/python"
VERSION_FILE="$PROJECT_PATH/othello/version.py"

# Read Python project version number from pyproject.toml
get_pyproject_version_number() {
    # note: `tomllib` requires Python 3.11+
    uv run python -c 'with open("pyproject.toml", "rb") as f: \
        import tomllib; \
        project = tomllib.load(f); \
        print(project["project"]["version"])'
}

update_version_information() {
    set_version_info
    VERSION_NUMBER="$(get_pyproject_version_number)"
    VERSION_STRING="Othello Python $VERSION_NUMBER $BUILD_TIME $GIT_BRANCH $GIT_COMMIT"
    {
        echo '"""'
        echo 'Version'
        echo 'Version information'
        echo 'Akseli Lukkarila'
        echo '2019-2026'
        echo '"""'
        echo ""
        echo "# Updated by build script, do not edit manually!"
        echo "BUILD_TIME = \"$BUILD_TIME\""
        echo "GIT_BRANCH = \"$GIT_BRANCH\""
        echo "GIT_COMMIT = \"$GIT_COMMIT\""
        echo "VERSION_NUMBER = \"$VERSION_NUMBER\""
        echo "VERSION_STRING = \"$VERSION_STRING\""
        echo ""
        echo ""
        echo 'def version_info() -> str:'
        echo '    """Return version info string."""'
        echo '    return VERSION_STRING'
    } > "$VERSION_FILE"
}

cd "$PROJECT_PATH"

if [ -z "$(command -v uv)" ]; then
    print_error_and_exit "uv not found in path"
fi

print_magenta "Building Othello Python..."
uv sync --all-groups --quiet
update_version_information

print_magenta "Version:"
uv run othello/othello.py --version
