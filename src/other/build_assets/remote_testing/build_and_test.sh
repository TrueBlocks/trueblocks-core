#!/bin/bash

# This script accepts 1 optional argument, which is passed to `make`
# and can be "tests" (default value) or "test-all".

# Set MAKE_TARGET to the first argument or "tests":
MAKE_TARGET="${1:-tests}"
RUN_SERVER=false

echo "Will perform $MAKE_TARGET"

if [ "$MAKE_TARGET" == "test-all" ]
then
    RUN_SERVER=true
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

    chifra serve > /dev/null 2>&1 &

    echo "Waiting for chifra to start up..."
    sleep 20

    echo "Checking if chifra is running..."
    pgrep chifra
    if [ $? -gt 0 ]
    then
        echo "Error: Chifra is not running"
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