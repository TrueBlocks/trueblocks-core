#!/usr/bin/env bash

#------------------------------------------------
# Assumes we start in the ./build folder
cd ..
REPO_ROOT=`pwd`
BUILD_FOLDER="$REPO_ROOT/build/"
DEST_FOLDER="$HOME/Library/Application Support/TrueBlocks/config/mainnet"
if [[ -d "$DEST_FOLDER" ]]
then
    echo ""
else
    DEST_FOLDER="$HOME/.local/share/trueblocks/config/mainnet"
fi

#------------------------------------------------
# Function to check command results and quit if failed
check_result() {
    local result=$1
    local message=${2:-"Build failed. Quitting."}
    if [ $result -ne 0 ]; then
        echo $message
        cd $BUILD_FOLDER
        exit $result
    fi
}

#------------------------------------------------
echo "Adding the tests submodule"
cd "$REPO_ROOT"
git submodule status tests

#------------------------------------------------
echo "Installing config files"
cd "$REPO_ROOT/build/other/install"
make

#------------------------------------------------
echo "Running go tests..."
cd "$REPO_ROOT/src/apps/chifra"
go test --tags integration ./...
check_result $? "Go tests failed. Quitting."

#------------------------------------------------
echo "Running cmake"
cd "$BUILD_FOLDER"
cmake ../src
check_result $? "CMake failed. Quitting."

#------------------------------------------------
echo "Running CMake"
# npm install -g prettier
# make generate
make -j 8
check_result $?

#------------------------------------------------
echo "Running the tests with testRunner"
TEST_MODE=true testRunner
check_result ${PIPESTATUS[0]}

#------------------------------------------------
echo "Done..."
exit 0
