#!/usr/bin/env bash

#------------------------------------------------
# Name:    build-khedra.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

#------------------------------------------------
# Determine the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
# Begin script execution here
cd "$SCRIPT_DIR/../khedra" || exit 1  # Exit if changing directory fails

if ! go build -o "../bin/khedra"; then
    echo "Build failed!" >&2
    exit 1
fi
