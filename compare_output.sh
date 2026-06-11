#!/bin/bash
set -eo pipefail

# Compare full program output between all implementations.
# Runs each executable in autoplay + test mode and diffs the output
# against the Rust implementation (the canonical reference).
# The first line (language banner) is skipped and ANSI codes are stripped
# unless --color is given, in which case escape codes must match too.

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

USAGE="Usage: $(basename "$0") [OPTIONS] [BOARD_SIZE...]

Compare program output against the Rust implementation.

OPTIONS: All options are optional
    --help | -h     Display these instructions.
    --color         Compare raw output including ANSI color codes.
"

cd "$DIR" > /dev/null

KEEP_COLOR=false
SIZES=()
while [ $# -gt 0 ]; do
    case "$1" in
        -h | --help)
            echo "$USAGE"
            exit 1
            ;;
        --color)
            KEEP_COLOR=true
            ;;
        *)
            SIZES+=("$1")
            ;;
    esac
    shift
done

if [ ${#SIZES[@]} -eq 0 ]; then
    SIZES=(4 5 6 7 8 9 10)
fi

if [ "$BASH_PLATFORM" = windows ]; then
    reference="rust/othello_rust.exe"
    executables=(
        cpp/othello_cpp.exe
        csharp/othello_csharp.exe
        go/othello_go.exe
        kotlin-native/othello_kotlin.exe
        kotlin/othello_kotlin.sh
        python/othello_python.sh
    )
else
    reference="rust/othello_rust"
    executables=(
        cpp/othello_cpp
        csharp/othello_csharp
        go/othello_go
        kotlin-native/othello_kotlin
        swift/othello_swift
        kotlin/othello_kotlin.sh
        python/othello_python.sh
    )
fi

# Print output for the given executable and board size,
# skipping the language banner on the first line.
normalized_output() {
    local exe="$1"
    local size="$2"
    if [ "$KEEP_COLOR" = true ]; then
        ./"$exe" "$size" -atl | tail -n +2
    else
        ./"$exe" "$size" -atl | sed $'s/\x1b\[[0-9;]*m//g' | tail -n +2
    fi
}

failures=0
for size in "${SIZES[@]}"; do
    print_magenta "Board size $size"
    reference_output=$(normalized_output "$reference" "$size")
    for exe in "${executables[@]}"; do
        if [ ! -e "$exe" ]; then
            print_yellow "    $exe (missing, skipped)"
            continue
        fi
        if diff <(echo "$reference_output") <(normalized_output "$exe" "$size") > /dev/null; then
            print_green "    $exe"
        else
            print_red "    $exe"
            failures=$((failures + 1))
        fi
    done
done

echo ""
if [ "$failures" -gt 0 ]; then
    print_red "$failures output mismatch(es) found"
    echo "Inspect with e.g.:"
    echo "  diff <(./$reference 8 -atl | sed \$'s/\\x1b\\[[0-9;]*m//g') <(./cpp/othello_cpp 8 -atl | sed \$'s/\\x1b\\[[0-9;]*m//g')"
    exit 1
fi
print_green "All outputs match"
