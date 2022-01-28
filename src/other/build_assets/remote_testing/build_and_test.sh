#!/bin/bash

# This script accepts 1 optional argument, which is passed to `make`
# and can be "tests" (default value) or "test-all".

# Set MAKE_TARGET to the first argument or "tests":
MAKE_TARGET="${1:-tests}"
RUN_SERVER=false
# generate a random port number between 8091 and 10_000
SRV_PORT=`shuf -n 1 -i 8091-10000`

echo "Will perform $MAKE_TARGET"

if [ "$MAKE_TARGET" == "test-all" ]
then
    RUN_SERVER=true
    echo "Serve required. Will use port $SRV_PORT"
    echo "[settings]
    api_provider=\"http://localhost:$SRV_PORT\"
    " > $HOME/.local/share/trueblocks/testRunner.toml
fi

cd /root/trueblocks-core/build
echo $(pwd)

# Set the correct PATH
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

# Run server if needed
if $RUN_SERVER
then
    if [ -n "$(pgrep chifra)" ]
    then
        echo "Error: Chifra serve is already running"
        exit 1
    fi

    chifra serve -p ":$SRV_PORT" 2>/dev/null &

    echo "Waiting for chifra server -p :$SRV_PORT to start..."
    sleep 15

    echo "Checking if chifra serve is running..."
    pgrep chifra
    if [ $? -gt 0 ]
    then
        echo "Error: Waited 15 seconds, chifra serve is not running"
        exit 2
    fi

    echo "Running tests..."
fi

# Run test
echo "make $MAKE_TARGET"
make "$MAKE_TARGET"
RESULT=$?

if $RUN_SERVER
then
    pkill chifra
fi

echo "Compressing and saving test/working"
tar -cJ --file /root/test_results/working.tar.xz ../test/working/

exit $RESULT
