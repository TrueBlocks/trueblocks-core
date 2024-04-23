#!/bin/bash

cd /root/trueblocks-core/build
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

SERVER_PORT=`shuf -n 1 -i 8091-10000`

echo "Home folder: $HOME"
echo "Whoami:      `whoami`"
echo "Server port: $SERVER_PORT"
echo "Pwd:         $(pwd)"

# starts the API server and runs the tests
export TB_TEST_API_SERVER=$SERVER_PORT
export TB_REMOTE_TESTING=true
make test-all
RESULT=$?

echo "Compressing and saving tests/working into $(pwd)"
tar -cJ --file /root/test_results/working.tar.xz ../tests/working/

exit $RESULT
