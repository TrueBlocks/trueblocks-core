#!/usr/bin/env bash

#------------------------------------------------
# Name:    rm-test-results.sh
# Purpose: This script cleans up old test results in a specific folder
# Crontab: 0 0 * * * /path/to/rm-test-results.sh --silent

# Set the directory name to test_results in the user's home directory
RESULT_FOLDER="$HOME/test_results"

# Check for silent mode flag
SILENT_MODE=$1

echo "Removing old test results from $RESULT_FOLDER"

# Check if directory exists and can be accessed
if [ ! -d "$RESULT_FOLDER" ] || [ ! -x "$RESULT_FOLDER" ]; then
    echo "Cannot access directory $RESULT_FOLDER"
    exit 1
fi

# Enter the directory
cd "$RESULT_FOLDER" || exit 1

echo "Current working directory: $(pwd)"

# List and remove files safely
# Use find command to list regular files only, without descending into subdirectories
TO_REMOVE=$(find . -maxdepth 1 -type f -printf '%T+ %p\n' | sort | head -n -3 | cut -d' ' -f2-)
if [ -z "$TO_REMOVE" ]; then
    echo "No old test results to remove."
else
    echo "Will remove these old test results:"
    echo "$TO_REMOVE"
    # Check if running in silent mode
    if [[ "$SILENT_MODE" == "--silent" ]]; then
        echo "$TO_REMOVE" | xargs rm -f --
        echo "Old test results removed in silent mode."
    else
        # Prompt before removing files
        echo "Proceed with removal? (y/n)"
        read -r confirm
        if [[ "$confirm" == "y" ]]; then
            echo "$TO_REMOVE" | xargs rm -f --
            echo "Old test results removed."
        else
            echo "Removal cancelled."
        fi
    fi
fi
