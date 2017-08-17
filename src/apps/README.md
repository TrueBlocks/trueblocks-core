# Apps

<hr>

The `apps` folder contains the primary applications built on the QuickBlocks libraries. This includes the decentralized blockchain scrapers `blockScrape`, `blockAcct` and `miniBlocks`; `chifra`, an automated way to create smart contract monitors; `ethprice` which pulls US dollar price data from the Internet; `ethslurp` which is a down-and-dirty centralized command line scraper utilizing *http://etherscan.io* that is very quick, but ultimately incomplete. Another folder, [tools](../tools) contains a collection of 'getters' to pull single (or ranges of) blocks, transactions, traces, receipts, etc.

1. [blockScrape](blockScrape) - The primary component of the QuickBlocks decentralized blockchain scraper. `blockScrape` runs periodically and retrieves blocks from the node software. After processing, this program then stores the blocks locally in a speed-optimized format.

2. [miniBlocks](miniBlocks) - `miniBlocks` compresses the transactional data found in each block removing as much unnecessary data as possible so that later retrieval of the transactions is as quick as possible.

3. [chifra](chifra) - `chifra` builds parsing libraries for particular sets of blockchain addresses. Developers may later link to these libraries to fully parse transactional input and event data.

4. [ethprice](ethprice) - `Ethprice` is a simple utility that pulls price data local to the hard drive for quicker retrieval. It is configurable to retrieve its data from any of five price feeds and collate or average the feeds.

5. [ethslurp](ethslurp) - This program pulls data from the http://etherscan.io APIs. It can be used in testing to verify that the results from QuickBlocks and the results from EtherScan are consistent.

Each of these programs is explained further in its own individual README.md files.

1. [grabABI](grabABI) - This is the basis of the `chafra` application. Given an Etheruem address, this tool first grabs the ABI from where ever it can find it (http://etherscan.io, ENS, etc.). It then parses the ABI and writes all the C++ necessary to build an Ethereum address monitor. `chafra` uses this tool extensively.

2. [makeClass](makeClass) - This tool is used by `grabABI` to write the actual C++ code generated during the `chafra` process. Many of the source files found in `etherlib` are also generated automatically using this tool.

3. [cacheMan](cacheMan) - This tool allows one to explore the a monitor's binary cache data file. It preforms various checks on the data. The tool provides options for truncating the cache and/or merging two caches into one which may be useful in certain circumstances.
