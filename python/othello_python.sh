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
    $PYTHON -c 'with open("pyproject.toml", "rb") as f: \
                import tomllib; \
                project = tomllib.load(f); \
                print(project["tool"]["poetry"]["version"])'
}

update_version_information() {
    set_version_info
    VERSION_NUMBER="$(get_pyproject_version_number)"
    {
        echo '"""'
        echo 'Version'
        echo 'Version information definitions'
        echo 'Akseli Lukkarila'
        echo '2019-2025'
        echo '"""'
        echo ""
        echo "# Updated by build script, do not edit manually!"
        echo "GIT_BRANCH = \"$GIT_BRANCH\""
        echo "GIT_COMMIT = \"$GIT_COMMIT\""
        echo "BUILD_TIME = \"$BUILD_TIME\""
        echo "VERSION_NUMBER = \"$VERSION_NUMBER\""
    } > "$VERSION_FILE"
}

cd "$PROJECT_PATH"

if [ -z "$(command -v poetry)" ]; then
    print_error_and_exit "poetry not found in path"
fi

check_and_set_python
update_version_information

# Pass arguments to program
poetry install --sync --quiet
poetry run python othello/othello.py "$@"
