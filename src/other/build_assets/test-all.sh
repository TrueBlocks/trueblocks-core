#!/usr/bin/env bash

cd ..
export QUICKBLOCKS=`pwd`
export INSTALL=$QUICKBLOCKS/build/other/install
export CHIFRA=$QUICKBLOCKS/src/apps/chifra
export BUILD_FOLDER=$QUICKBLOCKS/build/
export TEST_FOLDER=$QUICKBLOCKS/tests/

# The names.tab file is ours, so we can always replace it for testing
export NAMES_SOURCE=$QUICKBLOCKS/src/other/install/names/names.tab

cd "$INSTALL"
echo "Installing config files"
make

cd "$CHIFRA"
echo "Running go tests..."
go test --tags integration ./...
RESULT=$?
cd $BUILD_FOLDER
if [ $RESULT -ne 0 ]; then
   exit $RESULT
fi

#echo "Making..."
cd "$BUILD_FOLDER/"
cmake ../src
cd dev_tools
make -j 8
cd ..
make generate
make -j 8

cd $TEST_FOLDER/gold/dev_tools/testRunner

echo "Calling testRunner"
sdkTester
RESULT=${PIPESTATUS[0]}

cd $BUILD_FOLDER
export DEST_FOLDER="$HOME/Library/Application Support/TrueBlocks/config/mainnet"
if [[ -d $DEST_FOLDER ]]
then
    # do nothing
    echo ""
else
    export DEST_FOLDER="$HOME/.local/share/trueblocks/config/mainnet"
fi

# Put the original data back in place
cp -f "$NAMES_SOURCE" "$DEST_FOLDER"
touch "$DEST_FOLDER/names.tab"

cd $BUILD_FOLDER
echo "Done..."

exit $RESULT
