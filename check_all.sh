#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

executables=(
    othello_cpp/othello_cpp
    othello_csharp/othello_csharp
    othello_go/othello_go
    othello_rust/othello_rust
    othello_swift/othello_swift
    othello_python/othello_python.sh
    othello_kotlin/othello_kotlin.sh
)

for exe in "${executables[@]}"; do
    # Extract the part after the last '/'
    filename="${exe##*/}"

    # Remove the file extension if it exists
    filename="${filename%.*}"

    print_magenta "$filename"

    # Add .txt extension
    output_file="${filename}.txt"

    # Clear the file or create it if it doesn't exist
    rm -f "$output_file"

    # Loop from 4 to 10
    for N in {4..10}; do
        last_line=$(./"$exe" "$N" -atl | tail -n 1)
        formatted_number=$(printf "%02d" $N)
        echo "$formatted_number: $last_line"
        echo "$formatted_number: $last_line" >> "$output_file"
    done
done
