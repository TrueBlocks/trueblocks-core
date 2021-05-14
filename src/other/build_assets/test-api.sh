#!/usr/bin/env bash

# echo "Testing..."
cd $TEST_FOLDER/gold/dev_tools/testRunner

echo "Calling [testRunner $@]"
testRunner $@ | tee $BUILD_FOLDER/results.txt

cat $BUILD_FOLDER/results.txt | grep -v Skipping >x
mv -f x $BUILD_FOLDER/results.txt

cd $BUILD_FOLDER
