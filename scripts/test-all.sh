#!/usr/bin/env bash

#------------------------------------------------
# Assumes we start in the ./build folder
cd ..
REPO_ROOT=`pwd`
BUILD_FOLDER=$REPO_ROOT/build/
DEST_FOLDER="$HOME/Library/Application Support/TrueBlocks/config/mainnet"
if [[ -d $DEST_FOLDER ]]
then
    echo ""
else
    DEST_FOLDER="$HOME/.local/share/trueblocks/config/mainnet"
fi

#------------------------------------------------
echo "Installing config files"
INSTALL=$REPO_ROOT/build/other/install
cd "$INSTALL"
make

#------------------------------------------------
echo "Running go tests..."
CHIFRA=$REPO_ROOT/src/apps/chifra
cd "$CHIFRA"
go test --tags integration ./...
RESULT=$?
if [ $RESULT -ne 0 ]; then
   cd $BUILD_FOLDER
   exit $RESULT
fi

#------------------------------------------------
echo "Adding the tests submodule"
cd $REPO_ROOT
git submodule status tests

#------------------------------------------------
echo "Running cmake"
cd "$BUILD_FOLDER"
cmake ../src

#------------------------------------------------
echo "Running cmake"
# npm install -g prettier
# make generate
make -j 8
RESULT=$?
if [ $RESULT -ne 0 ]; then
   echo "Build failed. Quitting."
   exit $RESULT
fi

#------------------------------------------------
# The names.tab file is ours, so we can always replace it for testing
cp -f "$REPO_ROOT/src/other/install/names/names.tab" "$DEST_FOLDER"
touch "$DEST_FOLDER/names.tab"

#------------------------------------------------
echo "Running the tests with testRunner"
TEST_MODE=true testRunner
RESULT=${PIPESTATUS[0]}

#------------------------------------------------
echo "Done..."
exit $RESULT
