## Running an Ethereum Node

The greatest benefit from the Ethereum blockchain comes from running your own local node because the data is local and under consensus; however, this benefit comes at a huge price in disc space usage, processing requirements, and hassle. The TrueBlocks software assumes you are running a full local node. In fact, it assumes you are running an archive Parity node started with the following command line:

    parity --tracing on --pruning archive

Generally speaking this requires a fairly capable machine, although we've seen it run on Mac laptop. The blockchain's data itself takes up nearly 500 GB disc space (as of Nov. 2017). An SSD hard drive is strongly encouraged.

The current version of TrueBlocks stores block data in its cache only when a block is requested. If you request ten blocks, only ten blocks are be written to the cache. In this way, a minimum imposition is made in addition to the blockchain.

If you ask TrueBlocks to retrieve every block (as we do), every block is written to the cache. In our case, we want to do system-wide data analytics, and the TrueBlocks cache takes up an additional 500 GB.

We store our TrueBlocks cache on an external SSD hard drive. We've found [this SSD drive](https://www.amazon.com/Samsung-T5-Portable-SSD-MU-PA1T0B/dp/B073H552FJ/ref=sr_1_sc_3?ie=UTF8&qid=1507691185&sr=8-3-spell&keywords=Samsubg+1tb+ssd) to be quite excellent for this purpose. [Blockchain on a Stick](https://medium.com/@tjayrush/accounting-for-the-revolution-8822b28ccc16).

## Not Running an Ethereum Node

You may use a remote node if you wish or a testnet node by changing the rpcProvider option in the configuration file. The caching capabilities of TrueBlocks really shines in the remote scenario. Using TrueBlocks, we've seen performance increases of nearly two orders of magnitude over requesting the same data repeatedly from Infura. 

By default, so as to not over burden your hard drive, TrueBlocks does not cache every request. For example, the `getBlock` command only caches if you tell it to with the [currently undocumented] `--cache` option:

    getBlock 1001001 --cache
    
The Infura platform does not run their nodes with `--tracing on` nor `--pruning archive`; therefore, the behavior of operations such as getState, getTokens and getTrace, which are dependant on either tracing or archiving, is undefined.
