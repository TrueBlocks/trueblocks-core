********************
Technical Reference
********************

QuickBlocks is a collection of software libraries, command-line tools, and applications. The command-line tools, while very useful in their own right, are primarily intended to serve as instructional material helping you understand how to use the libraries to accomplish various tasks. The applications are more full-featured and provide useful functions such as providing a fully-decentralized block chain scraper (blockScrape), automatic generation of source code that comprises the libraries, and the libraries themselves which, while largely auto-generated provide most of the function. Each of these apsects of QuickBlocks is described below. We intend to make the libraries and tools open source. The applications will be commercial products.

Command-Line Tools
===================

The command-line tools present direct access to the Ethereum data structures while providing many options and export formats. They can be broken down into the following major groups:

Block-related tools
--------------------

    Four block-related tools deal primarily with the Ethereum block data structure. Each of these tools takes a **block_list** which may be one or more block numbers (as hex or integer), ranges
    of block numbers, special named blocks (see `whenBlock <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/whenBlock/README.md>`_), or any combination. Each of these
    tools is open source. The block-related tools are:

        `getBlock <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getBloom/README.md>`_,
        `getBloom <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getBloom/README.md>`_,
        `whenBlock <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/whenBlock/README.md>`_,
        `whereBlock <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/whereBlock/README.md>`_,

Transaction-related tools
--------------------------

    There are four transaction-related tools that deliver Ethereum transaction data structures. Each of these tools takes a **trans_list** which may be one or more
    transaction hashes, blocknumber / transID pairs, blockHash / transID pairs or any combination. Each of these tools is open source. The transaction-related tools are:

        `getTrans <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getTrans/README.md>`_,
        `getTrace <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getTrace/README.md>`_,
        `getLogs <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getLogs/README.md>`_,
        `getReceipt <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getReceipt/README.md>`_,

Account-related tools
----------------------

    Five account-related tools return data particular to accounts such as an accounts Ether or ERC20 token balance, an account's name, or whether or not it's a smart contract. These tools are also
    open source. The account-related tools are:

        `ethName <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/ethName/README.md>`_,
        `getAccounts <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getAccounts/README.md>`_,
        `getBalance <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getBalance/README.md>`_,
        `getTokenBal <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/getTokenBal/README.md>`_,
        `isContract <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/tools/isContract/README.md>`_,

Applications
=============

    QuickBlocks tools are intended primarily sample source code to help you understand how to program using the QuickBlocks libraries. QuickBlocks applications are more full-featured
    applications that accomplish specific tasks: scraping the chain, building cached databases for quick retrieval of the data, etc. The other thing that distinguishes applications from
    tools is that while the tools and the QuickBlocks libraries are open source, the applications are sold through our website.

blockScraper application
-------------------------

    The **blockScrape** application scrapes the Ethereum blockchain creating three different databases that help the other tools and applications quickly access and deliver the
    Ethereum shared ledger to your applications. This process is fully described in our `three white papers <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/other/papers/README.md>`_,.
    This tool is not open source. It is available for purchase from our website.

grabABI application
--------------------

    The **grabABI** program pulls your smart contract's ABI file from either Etherscan or (in the future) the ENS smart contract. Many things are possible with your smart contract's ABI file
    including automatic generation of source code needed to build a fully functional smart contract monitor / debugger. grabABI also is able to encode and decode the functional and event signatures
    needed to fully parse the blockchain data returned through the RPC. This tool is not open source. It is available for purchase from our website.

makeClass application
----------------------

Smart contract monitors
------------------------

Libraries
==========

utillib
--------

    The most basic functionality is contained in a library called **utillib**. This library consists of software code for carrying out common functions such as string and time
    manipulation; concurrency-protected data access; container classes such as lists, arrays and maps; and other utility functionality.

etherlib
---------

     The second library component is called etherlib. This library allows for reading, writing, and manipulation of application binary interface (ABI) files.
     This library also mirrors the blocks, transactions, receipts, traces, and accounts found in the blockchain data. It is in the etherlib that we interact
     directly with the blockchain via RPC. We do this in order to collect raw blockchain data, which is then enhanced so as to provide more useful data to
     higher-level components such as the tokenlib; the customized, per-smart-contract libraries; and the various applications. It is the job of etherlib to
     provide faster access to the data, and many speed optimizations, in addition to a collection of easy-to-use interfaces for traversing the blocks,
     transactions, and accounts, are contained in this library.

tokenlib / walletlib
---------------------

     The final two pre-compiled libraries are called tokenlib and walletlib, which implement support for the Ethereum ERC 20 token
     standard and popular multi-sig wallets.
