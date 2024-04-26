#!/usr/bin/env bash

#------------------------------------------------
# Name:    make-go-work.sh
# Purpose: Find go.mod files, save the directories
#          to a go.work file, and runs go work sync

#------------------------------------------------
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Navigate to the root of the repo
cd "$SCRIPT_DIR/.."

#------------------------------------------------
# Check if the go.work file already exists
GO_WORK_FILE="go.work"
if [ -f "$GO_WORK_FILE" ]; then
    echo "Error: $GO_WORK_FILE already exists. Remove it before re-running this script."
    exit 1
fi

#------------------------------------------------
# Echo in header content to the go.work file
echo "// Go Version" > $GO_WORK_FILE
echo "go 1.22" >> $GO_WORK_FILE
echo "" >> $GO_WORK_FILE
echo "use (" >> $GO_WORK_FILE

#------------------------------------------------
# Find all go.mod files in the src directory and
# add their directories to go.work.
find . -type f -name 'go.mod' | while read -r modfile; do
    moddir=$(dirname "$modfile")
    echo "	$moddir" >> $GO_WORK_FILE
done

#------------------------------------------------
# Close the file, and run go work sync
echo ")" >> $GO_WORK_FILE
go work sync

#------------------------------------------------
echo "Created go.work with these contents."
cat go.work
cd - 2>&1 > /dev/null
exit 0
