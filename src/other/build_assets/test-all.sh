#!/usr/bin/env bash

cd ..
export QUICKBLOCKS=`pwd`
export BUILD_FOLDER=$QUICKBLOCKS/build/
export TEST_FOLDER=$QUICKBLOCKS/test/
export NAMES_FOLDER=$QUICKBLOCKS/src/other/install/names/

#echo "Making..."
cd "$BUILD_FOLDER/"
cmake ../src
cd dev_tools
make -j 8
cd ..
make generate finish
make -j 8

test-api.sh --filter all --mode both --clean --report $@

cp -f "$NAMES_FOLDER/names.tab" "$HOME/Library/Application Support/TrueBlocks/names/"
touch "$HOME/Library/Application Support/TrueBlocks/names/names.tab"
rm -fR "$HOME/Library/Application Support/TrueBlocks/cache/names"

cd $BUILD_FOLDER
echo "Done..."
