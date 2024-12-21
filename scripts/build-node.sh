#!/usr/bin/env bash

#------------------------------------------------
# Name:    build-node.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

#------------------------------------------------
# Determine the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#------------------------------------------------
# Begin script execution here
cd "$SCRIPT_DIR/../node" || exit 1  # Exit if changing directory fails

echo "Built target node/trueblocks-node"
if ! go build -o "../../bin/trueblocks-node"; then
    echo "Build failed!" >&2
    exit 1
fi
