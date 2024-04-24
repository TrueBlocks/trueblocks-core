#!/usr/bin/env bash

#------------------------------------------------
# Name:    build-and-test.sh
# Purpose: This script builds a project, runs tests,
#          and archives the results.

#------------------------------------------------
# Navigate to the build folder and set the PATH
if ! cd /root/trueblocks-core/build; then
    echo "Failed to change to directory /root/trueblocks-core/build."
    exit 1
fi
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

#------------------------------------------------
# Get an unused port for the API server
export TB_TEST_API_SERVER=$(shuf -n 1 -i 8091-10000)
export TB_REMOTE_TESTING=true

#------------------------------------------------
# Display environment to the console
echo "Home folder: $HOME"
echo "Whoami:      $(whoami)"
echo "Server port: $TB_TEST_API_SERVER"
echo "cwd:         $(pwd)"

#------------------------------------------------
# Execute the tests
make test-all
RESULT=$?

#------------------------------------------------
# Compress and save the test directory
echo "Compressing and saving tests/working into $(pwd)"
tar -cJf /root/test_results/working.tar.xz ../tests/working/

exit $RESULT
