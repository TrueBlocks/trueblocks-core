#!/bin/bash

# This script accepts 1 optional argument, which is passed to `make`
# and can be "tests" (default value) or "test-all".

# Set MAKE_TARGET to the first argument or "tests":
MAKE_TARGET="${1:-tests}"
RUN_SERVER=false

if [ "$MAKE_TARGET" == "test-all" ]
then
    RUN_SERVER=true
fi

echo $(pwd)

# Build the binaries
mkdir build && cd build
cmake ../src
make -j 4

# Set the correct PATH
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

# Run server if needed
if $RUN_SERVER
then
    chifra serve &

    echo "Waiting for chifra to start up..."
    sleep 5

    echo "Checking if chifra is running..."
    pgrep -u $USER flame
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
    killall flame
fi

exit $RESULT