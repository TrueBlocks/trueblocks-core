## blockScrape

***
**Note:** This tool is availble through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

`blockScrape` queries your local node (or the ${FALLBACK} node if configured) using the RPC interface reading each block from any EVM-based blockchain. After extensive optimizations to the data, including determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized database for fast retrieval. By doing as much work as possible prior to storage, QuickBlocks is able to achieve significant increases in speed of retrieval over the node.

Using operating system tools such as Linux's `cron` you can easily maintain a  constantly fresh QuickBlocks database. Using QuickBlocks `display strings` technology, it is even easy to populate a regular web 2.0 database and from there a full featured website representing the full state of your smart contract.

[{USAGE_TABLE}][{FOOTER}]
w
