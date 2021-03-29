#!/usr/bin/env bash

export EXPLORER_FOLDER=$QUICKBLOCKS/../trueblocks-explorer/

# touch a file letting the servers (js and go) know that we're running in test mode
touch /tmp/test-api
if [ -d ${EXPLORER_FOLDER} ]; then
    # restart the javascript server
    touch $EXPLORER_FOLDER/api/server.js
fi

# echo "Testing..."
cd $TEST_FOLDER/gold/dev_tools/testRunner

echo "Calling [testRunner $@]"
testRunner $@ | tee $BUILD_FOLDER/results.txt

cat $BUILD_FOLDER/results.txt | grep -v Skipping >x
mv -f x $BUILD_FOLDER/results.txt

# echo "Clean up..."
rm -f /tmp/test-api
if [ -d ${EXPLORER_FOLDER} ]; then
    # restart the javascript server
    touch $EXPLORER_FOLDER/api/server.js
fi

cd $BUILD_FOLDER
