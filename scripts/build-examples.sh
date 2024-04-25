#!/usr/bin/env bash

#------------------------------------------------
# Name:    build-examples.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

#------------------------------------------------
# Function to build a Go program
build_target() {
    echo "Built target $1"
    (cd "$1" && go build -o "../../bin/examples/$1")
}

#------------------------------------------------
# Begin script execution here
cd examples || exit 1  # Exit if changing directory fails

# Find directories containing a 'go.mod' file, search only direct children
find . -maxdepth 2 -type f -name 'go.mod' -exec dirname {} \; | while read -r dir; do
    build_target "$(basename "$dir")"
done
