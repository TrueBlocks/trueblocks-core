#!/usr/bin/env bash

#------------------------------------------------
# Name:    test-all.sh
# Purpose: This script sets up the build environment,
#          builds the project, and runs the tests.

set -e

#------------------------------------------------
# Assumes we start in the ./build folder
cd ..
REPO_ROOT=$(pwd)
BUILD_FOLDER="${REPO_ROOT}/build"
DEST_FOLDER="$HOME/Library/Application Support/TrueBlocks/config/mainnet"
if [[ ! -d "$DEST_FOLDER" ]]; then
    DEST_FOLDER="$HOME/.local/share/trueblocks/config/mainnet"
fi

#------------------------------------------------
echo "Installing config files..."
INSTALL="${REPO_ROOT}/build/other/install"
make -C "$INSTALL"

#------------------------------------------------
echo "Running Go integration tests..."
CHIFRA="${REPO_ROOT}/src/apps/chifra"
(cd "$CHIFRA" && TB_NO_PROVIDER_CHECK=true go test --tags integration ./...)

#------------------------------------------------
echo "Running Go integration tests..."
NODE="${REPO_ROOT}/node"
(cd "$NODE" && TB_NO_PROVIDER_CHECK=true go test ./...)

#------------------------------------------------
echo "Reporting on the status of the tests submodule. No update..."
git -C "$REPO_ROOT" submodule status tests

#------------------------------------------------
echo "Running CMake build process..."
cmake -S "$REPO_ROOT/src" -B "$BUILD_FOLDER"
# npm install -g prettier
# make generate
make -C "$BUILD_FOLDER" -j 8

#------------------------------------------------
echo "Copying necessary files for testing..."
cp -f "$REPO_ROOT/src/other/install/names/names.tab" "$DEST_FOLDER"
touch "$DEST_FOLDER/names.tab"

#------------------------------------------------
echo "Running the tests with testRunner..."
(cd "$BUILD_FOLDER" && TEST_MODE=true testRunner)

echo "Done..."
