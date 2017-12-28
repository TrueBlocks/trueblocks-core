**********
Quickstart
**********

Start with the `installation instructions <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/other/install/INSTALL.md>`_ (which are slightly different per operating system). Here's the instructions for Ubuntu:

::

    (sudo) apt-get update
    (sudo) apt-get upgrade
    (sudo) apt-get install build-essential
    (sudo) apt-get install libcurl3-dev
    (sudo) apt-get install cmake
    (sudo) apt-get install python python-dev

Next compile, build, and install the executable files:

::

    git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .
    cd quickBlocks
    mkdir build
    cd build
    cmake ../src
    make
    (sudo) make install

Finally, play with some `sample code <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/samples>`_. Here's some sample code that uses one of QuickBlocks' `forEvery` functions to visit each block in the chain and print out its logsBloom. We present it with no explanation.

::

    #include "etherlib.h"
    #include "options.h"

    //--------------------------------------------------------------
    int main(int argc, const char *argv[]) {

        etherlib_init();

        COptions opt;
        opt.parseArguments(argc, argv);

        forEveryBlock ( displayBloom, &opt, opt.start, opt.nBlocks, opt.skip );

        return 0;
    }

    //-------------------------------------------------------------
    bool displayBloom(CBlock& block, void *data)
    {
        cout << block.blockNumber << fromBloom(block.logsBloom) << "\n";

        return true;
    }
