#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

PROJECT_PATH="$REPO_ROOT/othello_python"
VERSION_FILE="$PROJECT_PATH/othello/version.py"

check_and_set_python() {
    # Check Python is found on path and set PYTHON variable to it
    if [ -n "$(command -v python3)" ]; then
        PYTHON=$(which python3)
    elif [ -n "$(command -v python)" ]; then
        PYTHON=$(which python)
    else
        print_error_and_exit "Python not found in path"
    fi

    echo "$($PYTHON --version) from $PYTHON"

    if ! $PYTHON -c 'import sys; sys.exit(1) if sys.version_info < (3, 11) else sys.exit(0)'; then
        print_error_and_exit "Python 3.11+ required"
    fi
}

# Read Python project version number from pyproject.toml
get_pyproject_version_number() {
    # note: `tomllib` requires Python 3.11+
    python -c 'with open("pyproject.toml", "rb") as f: \
                import tomllib; \
                project = tomllib.load(f); \
                print(project["tool"]["poetry"]["version"])'
}

update_version_information() {
    print_yellow "Updating version file: $VERSION_FILE"
    set_version_info
    VERSION_NUMBER="$(get_pyproject_version_number)"
    {
        echo '"""'
        echo 'Version'
        echo 'Version information definitions'
        echo 'Akseli Lukkarila'
        echo '2019-2023'
        echo '"""'
        echo "BRANCH = \"$GIT_BRANCH\""
        echo "COMMIT = \"$GIT_HASH\""
        echo "DATE = \"$BUILD_TIME\""
        echo "VERSION_NUMBER = \"$VERSION_NUMBER\""
    } > "$VERSION_FILE"
}

cd "$PROJECT_PATH" > /dev/null

check_and_set_python
update_version_information

# Pass arguments to program
$PYTHON "$PROJECT_PATH/othello/othello.py" "$@"
