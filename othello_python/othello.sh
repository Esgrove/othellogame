#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

# Check Python is found on path and set PYTHON variable to it
if [ -n "$(command -v python3)" ]; then
    PYTHON=$(which python3)
elif [ -n "$(command -v python)" ]; then
    PYTHON=$(which python)
else
    print_error_and_exit "Python not found in path"
fi

echo "$($PYTHON --version) from $PYTHON"

$PYTHON othello/othello.py
