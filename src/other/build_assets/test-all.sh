#!/usr/bin/env bash

cd ..
export QUICKBLOCKS=`pwd`
export INSTALL=$QUICKBLOCKS/build/other/install
export CHIFRA=$QUICKBLOCKS/src/apps/chifra
export BUILD_FOLDER=$QUICKBLOCKS/build/
export TEST_FOLDER=$QUICKBLOCKS/test/

# We will use these to replace this data in case it's modified during testing
export NAMES_SOURCE=$QUICKBLOCKS/src/other/install/names/names.tab
export NAMES_CUSTOM=$QUICKBLOCKS/src/other/install/names/names_custom.tab

cd "$INSTALL"
echo "Installing config files"
make

cd "$CHIFRA"
echo "Running go tests..."
go test --tags integration ./...
if [ $? -ne 0 ]; then
   cd $BUILD_FOLDER
   exit $?
fi
cd $BUILD_FOLDER

#echo "Making..."
cd "$BUILD_FOLDER/"
cmake ../src
cd dev_tools
make -j 8
cd ..
make generate finish
make -j 8

~/.local/bin/chifra/test/test-api.sh --filter all --mode both --clean --report $@
RESULT=$?

# Special case for customized customized names
if [[ -f "$HOME/Desktop/names_custom.tab" ]]
then
    export NAMES_CUSTOM="$HOME/Desktop/names_custom.tab"
fi

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
cp -f "$NAMES_CUSTOM" "$DEST_FOLDER"
touch "$DEST_FOLDER/names.tab"
touch "$DEST_FOLDER/names_custom.tab"

cd $BUILD_FOLDER
echo "Done..."

exit $RESULT
