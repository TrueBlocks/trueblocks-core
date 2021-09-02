#!/usr/bin/env bash

cd ..
export QUICKBLOCKS=`pwd`
export BUILD_FOLDER=$QUICKBLOCKS/build/
export TEST_FOLDER=$QUICKBLOCKS/test/

#echo "Making..."
cd $BUILD_FOLDER/
cmake ../src
cd dev_tools
make -j 8
cd ..
make -j 8
~/.local/bin/chifra/test/test-api.sh --filter fast --mode cmd $@

cd $BUILD_FOLDER
echo "Done..."
