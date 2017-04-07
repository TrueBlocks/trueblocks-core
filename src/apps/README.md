# quickBlocks Apps

The `apps` folder contains the primary applications that QuickBlocks needs to accomplish its goals.

1. [blockScrape](blockScrape) - The primary component of the QuickBlocks decentralized blockchain scraper. `blockScrape` runs periodically and retrieves blocks from the node software. After processing, this program then stores the blocks locally in a speed-optimized format.

2. [miniBlocks](miniBlocks) - `miniBlocks` compresses the transactional data found in each block removing as much unnecessary data as possible so that later retrieval of the transactions is as quick as possible.

3. [getBlock](getBlock) / [getTrans](getTrans) / [getReceipt](getReceipt) / [getLog](getLog) / [getCode](getCode) / [getBalance](getBalance) - Each of these programs retrieves a single (or series of) blocks / transactions / receipts / logs / codes / balances from the specified item.

4. [chifra](chifra) - `chifra` builds parsing libraries for particular sets of blockchain addresses. Developers may later link to these libraries to fully parse transactional input and event data.

5. [ethprice](ethprice) - `Ethprice` is a simple utility that pulls price data local to the hard drive for quicker retrieval. It is configurable to retrieve its data from any of five price feeds and collate or average the feeds.

6. [ethslurp](ethslurp) - This program pulls data from the http://etherscan.io APIs. It can be used in testing to verify that the results from QuickBlocks and the results from EtherScan are consistent.

Each of these programs is explained further in its own individual README.md files.
