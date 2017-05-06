## Tools

The `tools` folder contains command-line tools that serve primarily as illustrative examples of using the QuickBlocks [software libraries](../libs). You will find that the source code for each of the individual tools is quite simple.

While primarily illustrative, these tools may be useful in other ways as well, for example, in testing and scripting scenarios. Each is fully explained further in its own README.md file.

#### Applications

+ [getBlock](getBlock) / [getTrans](getTrans) / [getReceipt](getReceipt) / [getLogs](getLogs) - These programs allows you to retrieve particular parts of the Ethereum blockchain. By default, each of these tools retrieves a cached version of the blockchain data from QuickBlocks, however, they may also be used to query the Ethereum node (i.e. parity) directly. We use these tools in various testing and scripting scenarios.

+ [getBalance](getBalance) - Unlike the other `get` commands in this folder, `getBalance` operates only against the Ethereum node. The program retrieves balances from one or more addresses at a given block. We use this program in testing our [monitors](../monitors) and the ensure that our cacheing mechanisms work properly.

+ [isContract](isContract) - This app returns `true` or `false` depending if the address is a smart contract or not. It may also be used to pull the actual byte code of the contract, if present. Furthermore, it can be used to compare two Ethereum addresses to see if they hold the same byte code.

+ [whenBlock](whenBlock) - This surprisingly handy tool allows you to enter either a block number and it returns the date of that block, or a date and it returns the block just prior to the date. It makes exploring and analyzing the blockchain much easier because you don't have to remember block numbers.

+ [ethName](ethName) - This simple tool provides a rudimentary service attaching Ethereum address to names and visa versa. It may be used as a simple reminder of ethereum addresses vs. user-specified names.

#### Scripts

+ [scripts](scripts) - A collection of simple bash shell scripts that make working with the blockchain simpler.

    - **4byte** - This script querys the [4byte directory](https://www.4byte.directory/) looking for matching function and/or event signatures given a function or event name, or visa-versa.

    - **ethgoog** - This script searches google for a given Ethereum address while removing any search engine results matching well-known blockchain scrapers. This makes it easier to find references to an addresses if one it trying to attach that addresses to a human name.

    - **ethscan** - This script opens [Etherscan](http://etherscan.io) with the given block number, block hash, transaction hash or Ethereum address. While it is a centralized solution, Etherscan provides an easily accessible list of transactions for an address.

    - **openRPC** / **openYP** - We use these two simple scripts to open web pages with much needed reference materials such as the RPC specifications and the Yellow Paper.
