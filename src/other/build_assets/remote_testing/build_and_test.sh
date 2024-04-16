#!/bin/bash

cd /root/trueblocks-core/build
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

SERVER_PORT=`shuf -n 1 -i 8091-10000`

echo "Running make test-all"
echo "Home folder: $HOME"
echo "Whoami:      `whoami`"
echo "Server port: $SERVER_PORT"
echo "Pwd:         $(pwd)"

# warms up the cache, makes sure there's an installation
chifra blocks 1>/dev/null 2>&1
RESULT=$?
if [ $RESULT -ne 0 ]; then
   cd $BUILD_FOLDER
   exit $RESULT
fi

# starts the API server and runs the tests
export TB_TEST_API_SERVER=$SERVER_PORT
export TB_REMOTE_TESTING=true
make && make test-sdk
RESULT=$?

echo "Compressing and saving test/working into $(pwd)"
tar -cJ --file /root/test_results/working.tar.xz ../test/working/

exit $RESULT
