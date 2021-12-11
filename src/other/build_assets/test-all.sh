#!/usr/bin/env bash

cd ..
export QUICKBLOCKS=`pwd`
export CHIFRA=$QUICKBLOCKS/src/apps/chifra
export BUILD_FOLDER=$QUICKBLOCKS/build/
export TEST_FOLDER=$QUICKBLOCKS/test/
export NAMES_SOURCE=$QUICKBLOCKS/src/other/install/names/names.tab
export NAMES_CUSTOM=$QUICKBLOCKS/src/other/install/names/names_custom.tab

cd $CHIFRA
echo "Running go tests..."
go test ./...
if [ $? -ne 0 ]; then
   cd -
   exit $?
fi
cd -

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

export DEST_FOLDER=$HOME/Library/Application\ Support/TrueBlocks/names
if [[ -d $DEST_FOLDER ]]
then
    # do nothing
    echo ""
else
    export DEST_FOLDER="$HOME/.local/share/trueblocks/names"
fi

cp -f "$NAMES_SOURCE" "$DEST_FOLDER"
cp -f "$NAMES_CUSTOM" "$DEST_FOLDER"
touch "$DEST_FOLDER/names.tab"
touch "$DEST_FOLDER/names_custom.tab"

cd $BUILD_FOLDER
echo "Done..."

exit $RESULT
