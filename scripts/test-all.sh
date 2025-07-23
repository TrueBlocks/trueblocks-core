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
DEST_FOLDER="${HOME}/Library/Application Support/TrueBlocks/config/mainnet"
if [[ ! -d "${DEST_FOLDER}" ]]; then
    DEST_FOLDER="${HOME}/.local/share/trueblocks/config/mainnet"
fi

#------------------------------------------------
echo "Installing config files..."
INSTALL="${REPO_ROOT}/build/other/install"
make -C "${INSTALL}"

#------------------------------------------------
# If these variables are not set, we do go test...
if [ -z "${TB_TEST_ENABLED}" ] && [ -z "${TB_TEST_FILTER}" ]; then
    #------------------------------------------------
    echo "Running Go integration tests..."
    CHIFRA="${REPO_ROOT}/src/apps/chifra"
    cd "${CHIFRA}" || exit 1  # Ensure we actually enter the directory
    TB_NO_PROVIDER_CHECK=true go test --tags integration ./...
    STATUS=$?
    cd - > /dev/null  # Return to the previous directory (optional)
    if [ ${STATUS} -ne 0 ]; then
      echo "Chifra tests failed."
      exit ${STATUS}
    fi

    #------------------------------------------------
    if [ "${TB_NO_KHEDRA_TEST}" != "true" ]; then
      echo "Running Go integration tests..."
      KHEDRA="${REPO_ROOT}/khedra"
      cd "${KHEDRA}" || exit 1  # Ensure we actually enter the directory
      TB_NO_PROVIDER_CHECK=true go test ./...
      STATUS=$?
      cd - > /dev/null  # Return to the previous directory (optional)
      if [ ${STATUS} -ne 0 ]; then
        echo "Khedra tests failed."
        exit ${STATUS}
      fi
    else
      echo "Skipping Khedra tests because TB_NO_KHEDRA_TEST is true."
    fi
fi

#------------------------------------------------
echo "Reporting on the status of the tests submodule. No update..."
git -C "${REPO_ROOT}" submodule status tests

#------------------------------------------------
echo "Running CMake build process..."
cmake -S "${REPO_ROOT}/src" -B "${BUILD_FOLDER}"
make -C "${BUILD_FOLDER}" -j 8

#------------------------------------------------
echo "Copying necessary files for testing..."
cp -f "${REPO_ROOT}/src/other/install/names/names.tab" "${DEST_FOLDER}"
touch "${DEST_FOLDER}/names.tab"

#------------------------------------------------
echo "Running the tests with testRunner..."
(cd "${BUILD_FOLDER}" && TEST_MODE=true testRunner)

echo "Done..."
