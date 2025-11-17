#!/bin/bash
set -eo pipefail

# Import common functions
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=./common.sh
source "$DIR/common.sh"

cd "$DIR" > /dev/null

if [ "$BASH_PLATFORM" = windows ]; then
    executables=(
        cpp/othello_cpp.exe
        csharp/othello_csharp.exe
        go/othello_go.exe
        kotlin-native/othello_kotlin.exe
        rust/othello_rust.exe
        swift/othello_swift.exe
        kotlin/othello_kotlin.sh
        python/othello_python.sh
    )
else
    executables=(
        cpp/othello_cpp
        csharp/othello_csharp
        go/othello_go
        kotlin-native/othello_kotlin
        rust/othello_rust
        swift/othello_swift
        kotlin/othello_kotlin.sh
        python/othello_python.sh
    )
fi

output_files=()

for exe in "${executables[@]}"; do
    # Extract the part after the last '/'
    filename="${exe##*/}"

    # Remove the file extension if it exists
    filename="${filename%.*}"

    print_magenta "$filename"

    # Add .txt extension
    output_file="${filename}.txt"

    output_files+=("$output_file")

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

echo ""
echo "Diffing outputs..."
for left in "${output_files[@]}"; do
    print_magenta "$left"
    for right in "${output_files[@]}"; do
        if ! diff --suppress-common-lines -y "$left" "$right" > /dev/null; then
            print_red "    $right"
        else
            print_green "    $right"
        fi
    done
done
