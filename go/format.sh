#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../common.sh
source "$DIR/../common.sh"

USAGE="Usage: $0 [OPTIONS]

Format Go code.

OPTIONS: All options are optional
  -h | --help         Display these instructions
  -c | --check        Check formatting
  -x | --verbose      Display commands being executed
"
export USAGE

CHECK=false
while [ $# -gt 0 ]; do
    case "$1" in
        -h | --help)
            print_usage_and_exit
            ;;
        -c | --check)
            CHECK=true
            ;;
        -x | --verbose)
            set -x
            ;;
        *)
            print_warn "Unknown argument '$1'"
            print_usage_and_exit
            ;;
    esac
    shift
done

if [ "$CHECK" = true ]; then
    UNFORMATTED=$(gofmt -l .)
    if [ -n "$UNFORMATTED" ]; then
        print_error "Code is not formatted correctly with gofmt"
        echo "$UNFORMATTED"
        exit 1
    else
        print_green "All Go files are properly formatted"
    fi
else
    print_magenta "Formatting Go code..."
    gofmt -s -w .
fi
