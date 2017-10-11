## [{NAME}]

***
**Note:** This tool is availble through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

The `blockScrape` program, using your local node's RPC or IPC, reads each block from any EVM-based blockchain and stores it for later retrieval. After extensive optimizations to the data, including determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized binary database for fast retrieval. By doing as much work as possible before storage, we see significant increase in the speed of retrieval.

If you automate `blockScrape` using something like `cron` it will keep the binary database fresh as per the blockchain its scraping. A sister program, called `miniBlocks` uses the data created by `blockScrape` to build an ultra-light-weight less-detailed, but much faster version of the data.

[{USAGE_TABLE}][{FOOTER}]
