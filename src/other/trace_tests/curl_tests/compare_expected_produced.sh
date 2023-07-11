#!/bin/bash

# Define directories
dir1="expected"
dir2="produced"
dir3="diffs"

# Ensure that dir3 exists and is empty
mkdir -p "$dir3"
rm -f "$dir3"/*.diff

# Loop over all files in dir1
for file in "$dir1"/*; do
    # Get the base name of the file (removing directory path)
    basefile=$(basename "$file")

    # Check if the file exists in dir2
    if [[ -f "$dir2/$basefile" ]]; then
        # If the file exists, run the git diff
        diff_output=$(git diff --no-index "$dir1/$basefile" "$dir2/$basefile")

        # Check if the diff output is non-empty
        if [[ -n "$diff_output" ]]; then
            # If it is non-empty, save it to a file in dir3
            echo "$diff_output" > "$dir3/$basefile.diff"
        fi
    else
        # If the file does not exist in dir2, print a warning
        echo "File $basefile does not exist in $dir2"
    fi
done
