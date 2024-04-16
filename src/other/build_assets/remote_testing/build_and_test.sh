#!/bin/bash

SERVER_PORT=`shuf -n 1 -i 8091-10000`

cd /root/trueblocks-core/build
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

echo "Running make test-all"
echo "Home folder: $HOME"
echo "Whoami:      `whoami`"
echo "Server port: $SERVER_PORT"
echo "Path:        $(pwd)"
echo "Ssh sessions:"
ps -ef | grep autossh

# if [ -n "$(pgrep chifra)" ]
# then
#     echo "Error: chifra daemon is already running"
#     exit 1
# fi

chifra blocks 1>/dev/null 2>&1
# TEST_MODE=true chifra daemon -u ":$SERVER_PORT" 2>/dev/null &

# echo "Sleeping for 10 seconds until API servers starts..."
# sleep 10

# echo "Checking if chifra daemon is running..."
# pgrep chifra
# if [ $? -gt 0 ]
# then
#     echo "Error: Waited for chifra daemon but it's not running. Quitting..."
#     exit 2
# fi

# echo "Running tests..."
export TB_TEST_API_SERVER=$SERVER_PORT
export TB_REMOTE_TESTING=true
# make test-all
# RESULT=$?
make && make test-sdk
RESULT=$?

# echo "Cleaning up..."
# pkill chifra

echo "Compressing and saving test/working into $(pwd)"
tar -cJ --file /root/test_results/working.tar.xz ../test/working/

exit $RESULT
