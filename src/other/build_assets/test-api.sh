#!/usr/bin/env bash

# echo "Testing..."
cd $TEST_FOLDER/gold/dev_tools/testRunner

echo "Calling [testRunner $@]"
testRunner $@ | tee $BUILD_FOLDER/results.txt
RESULT=${PIPESTATUS[0]}

cat $BUILD_FOLDER/results.txt | grep -v Skipping >x
mv -f x $BUILD_FOLDER/results.txt

cd $BUILD_FOLDER

if [ $RESULT -gt 0 ]; then
    echo "Tests failed"
    exit 1
fi
