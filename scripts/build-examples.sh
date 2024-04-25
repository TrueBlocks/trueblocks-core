#!/usr/bin/env bash

#------------------------------------------------
# Name:    build-examples.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

#------------------------------------------------
# Determine the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
# Function to build a Go program
build_target() {
    echo "Built target examples/$1"
    (cd "$1" && go build -o "../../bin/examples/$1")
}

#------------------------------------------------
# Begin script execution here
cd "$SCRIPT_DIR/../examples" || exit 1  # Exit if changing directory fails

# Find directories containing a 'go.mod' file, search only direct children
find . -maxdepth 2 -type f -name 'go.mod' -exec dirname {} \; | while read -r dir; do
    build_target "$(basename "$dir")"
done
