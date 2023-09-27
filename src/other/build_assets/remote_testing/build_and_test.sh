#!/bin/bash

# This script, which is called from the Dockerfile, accepts 1 optional argument, which is
# passed to `make` and can be "tests" (default value) or "test-all".

# Set MAKE_TARGET to the first argument or "tests":
MAKE_TARGET="${1:-tests}"
RUN_SERVER=false
# generate a random port number between 8091 and 10_000
SRV_PORT=`shuf -n 1 -i 8091-10000`

echo "Will perform $MAKE_TARGET"
echo "Home folder: $HOME"
echo "Whoami: `whoami`"

if [ "$MAKE_TARGET" == "test-all" ]
then
    RUN_SERVER=true
    echo "Server required. Will use port $SRV_PORT"
    echo "[settings]
api_provider=\"http://localhost:$SRV_PORT\"
" > $HOME/.local/share/trueblocks/testRunner.toml
fi

cd /root/trueblocks-core/build
echo $(pwd)

# Set the correct PATH
export PATH=$(pwd)/../bin:$(pwd)/../bin/test:$PATH

echo "----- We need autossh connections (should see at least two, maybe three -----"
ps -ef | grep autossh

# Run server if needed
if $RUN_SERVER
then
    if [ -n "$(pgrep chifra)" ]
    then
        echo "Error: chifra daemon is already running"
        exit 1
    fi

    TEST_MODE=true chifra daemon -p ":$SRV_PORT" 2>/dev/null &

    echo "Waiting for chifra daemon -p :$SRV_PORT to start..."
    sleep 15

    echo "Checking if chifra daemon is running..."
    pgrep chifra
    if [ $? -gt 0 ]
    then
        echo "Error: Waited 15 seconds, chifra daemon is not running. Quitting..."
        exit 2
    fi

    echo "Running tests..."
fi

# echo "----- Contents of testRunner.toml -----"
# cat $HOME/.local/share/trueblocks/testRunner.toml
# echo "---------------------------------------"
# echo
# echo "----- Contents of trueBlocks.toml -----"
# cat $HOME/.local/share/trueblocks/trueBlocks.toml
# echo "---------------------------------------"
# echo
# echo "----- ls -lR /home/unchained -----"
# ls -lR /home/unchained/
# echo "---------------------------------------"
# echo
# echo "----- ls -lR $HOME/.local/share/trueblocks/ -----"
# ls -lR $HOME/.local/share/trueblocks/
# echo "---------------------------------------"
# echo
# echo "----- 0xf503017d7baf7fbc0fff7492b751025c6a78179b --accounting --last_block 8860511 --articulate --ether --fmt json ----- "
# chifra export 0xf503017d7baf7fbc0fff7492b751025c6a78179b --accounting --last_block 8860511 --articulate --ether --fmt json
# echo "---------------------------------------"
# echo

# Run test
echo "make $MAKE_TARGET"
make "$MAKE_TARGET"
RESULT=$?

if $RUN_SERVER
then
    pkill chifra
fi

P=`pwd`
echo "Compressing and saving test/working into $P"
tar -cJ --file /root/test_results/working.tar.xz ../test/working/

exit $RESULT
