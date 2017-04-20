# Apps

The `apps` folder contains the primary applications built on the QuickBlocks libraries. This includes the decentralized blockchain scrapers `blockScrape` and `miniBlocks`; `chifra`, an automated way to create smart contract monitors; `ethprice` which pulls US dollar price data from the Internet; `ethslurp` which is a down-and-dirty centralized command line scraper utilizing *http://etherscan.io* that is very quick, but ultimately incomplete, and a collection of data 'getters' to pull single (or ranges) of blocks, transactions, etc.

1. [blockScrape](blockScrape) - The primary component of the QuickBlocks decentralized blockchain scraper. `blockScrape` runs periodically and retrieves blocks from the node software. After processing, this program then stores the blocks locally in a speed-optimized format.

2. [miniBlocks](miniBlocks) - `miniBlocks` compresses the transactional data found in each block removing as much unnecessary data as possible so that later retrieval of the transactions is as quick as possible.

3. [chifra](chifra) - `chifra` builds parsing libraries for particular sets of blockchain addresses. Developers may later link to these libraries to fully parse transactional input and event data.

4. [ethprice](ethprice) - `Ethprice` is a simple utility that pulls price data local to the hard drive for quicker retrieval. It is configurable to retrieve its data from any of five price feeds and collate or average the feeds.

5. [ethslurp](ethslurp) - This program pulls data from the http://etherscan.io APIs. It can be used in testing to verify that the results from QuickBlocks and the results from EtherScan are consistent.

6. [getBlock](getBlock) / [getTrans](getTrans) / [getReceipt](getReceipt) / [getLog](getLog) / [getBalance](getBalance) - Each of these programs retrieves a single (or series of) blocks / transactions / receipts / logs / codes / balances from the specified item. These tools may be useful in various shell scripting and testing scenarios.

7. [isContract](isContract) - This app returns `true` or `false` depending if the address is a smart contract or not. It can also pull the actual byte code of the contract, if present.

Each of these programs is explained further in its own individual README.md files.
