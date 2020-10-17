## Apps

The `apps` folder contains the primary applications built on the TrueBlocks libraries. This includes the decentralized blockchain scrapers `blockScrape`; `chifra`, an automated way to create smart contract monitors; `ethQuote` which pulls US dollar price data from remote sources; `ethslurp` which is a down-and-dirty centralized command line scraper utilizing http://etherscan.io.

The following applications are open source and available at our git hub:

- [**ethQuote**](ethQuote/README.md): A simple utility that pulls price data local from various sources. It is configurable to retrieve its data from any of a number of price feeds and collate or average the feeds. US Dollar prices can be used in tax reporting and current balance reporting tools.

- [**ethslurp**](ethslurp/README.md): This program pulls data from the http://etherscan.io APIs. It may be used to verify results from TrueBlocks, checking that they are consistent with EtherScan. Ultimately, this tool is incomplete in two ways: (1) it pulls data from a centralized source as opposed to a locally-running node, and (2) it ignores internal smart contract message calls.

- [**grabABI**](grabABI/README.md): This is the basis of the `chifra` application. Given an Etheruem address, this tool first grabs an ABI from one of numerous sources (http://etherscan.io, ENS, etc.). It then parses the ABI and automatically writes the C++ source code necessary to build an Ethereum address monitor. `chifra` uses `grabABI` and `makeClass` extensively.

- [**makeClass**](makeClass/README.md): This tool is used by `grabABI` to write the actual C++ code generated during the `chifra` process. Many of the source files found in `etherlib` were generated automatically using this tool.

The following tools are not available as open source. We intend to sell or license them through our website:

- [**blockScrape**](blockScrape/README.md): The primary component of the TrueBlocks decentralized blockchain scraper. `blockScrape` runs periodically and retrieves blocks from a locally-running Ethereum node. After processing, this program stores the blocks locally in a speed-optimized format. **Note:** This application will be delivered through our [website](http://quickblocks.io).

- [**chifra**](chifra/README.md): This tool builds parsing libraries for particular sets of blockchain addresses. Developers may later link to these libraries to fully parse transactional input and event data from the contract. We've called this data `articulated data` elsewhere. **Note:** This application will be delivered through our [website](http://quickblocks.io).
