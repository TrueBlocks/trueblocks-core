#!/usr/bin/env bash

# touch a file letting the servers (js and go) know that we're running in test mode
touch /tmp/test-api
if [ -d ${DOCKER_FOLDER} ]; then
    # restart the javascript server
    touch $DOCKER_FOLDER/api/server.js
fi

# echo "Testing..."
cd $TEST_FOLDER/gold/dev_tools/testRunner

testRunner $@ | tee $BUILD_FOLDER/results.txt

cat $BUILD_FOLDER/results.txt | grep -v Skipping >x
mv -f x $BUILD_FOLDER/results.txt

# echo "Clean up..."
rm -f /tmp/test-api
if [ -d ${DOCKER_FOLDER} ]; then
    # restart the javascript server
    touch $DOCKER_FOLDER/api/server.js
fi

cd $BUILD_FOLDER
