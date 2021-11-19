## NOTICE

This document is out of date. Please see our [website](http://trueblocks.io).

## Tools

The `tools` folder contains command-line programs that serve primarily as examples of using the TrueBlocks [libraries](../libs). You will find that the source code for each of the individual tools is quite simple. In a certain sense, each tool give you access to a different part of the Ethereum data structure.

<img src="../other/images/data_structures.png" style="border:1px orange solid">

While primarily illustrative, these tools are also useful. For example, you may use them in testing and scripting scenarios. Each is fully documented further in its own README.md file.

#### Tools

+ [getBlocks](getBlocks) / [getTrans](getTrans) / [getReceipts](getReceipts) / [getLogs](getLogs): These programs allow you to retrieve primary data structures of the Ethereum blockchain. By default, each tool retrieves its data from the running node, but you may instruct the tool to cache the blockchain data leading to significantly faster access on subsequent requests. We use these tools in various testing and scripting scenarios.

+ [getTraces](getTraces): Similar to the previous but give you access to much a lower level of Ethereum's data structures.

+ [getState](getState) / [getTokens](getTokens): Unlike the above commands, `getState` and `getTokens` operate directly against the node. These programs retrieve balances `ether` or `token` balances from one or more smart contracts given block number. We use these program in testing TrueBlocks [monitors](../monitors) and to ensure that our cacheing mechanisms work properly. This app also returns `true` or `false` depending if the address is a smart contract. It may also be used to pull the actual byte code of the contract, if present. Furthermore, it can be used to compare two Ethereum addresses to see if they hold the same byte code.

+ [whenBlock](whenBlock) - `whenBlock` is a surprisingly handy tool that allows you to enter either a block number and it returns that block's date, or a date and it returns the block just prior to that date. It makes exploring and analyzing the blockchain much easier because you don't have to remember block numbers.

+ [ethName](ethName) - This simple tool provides a rudimentary service attaching Ethereum address to names and visa versa. It may be used as a simple reminder of ethereum addresses vs. user-specified names. In the future, we will connect this to the ENS system.

#### Scripts

+ [scripts](scripts): The ./tools/scripts folder contains a collection of simple python scripts that make working with the blockchain simpler.

    - **ethgoog.py** - This script searches google for a given Ethereum address while removing any search engine results matching well-known blockchain scrapers. This makes it easier to find references to an addresses if one it trying to attach that addresses to a human name.

    - **ethscan.py** - This script opens [Etherscan](http://etherscan.io) with the given block number, block hash, transaction hash or Ethereum address. While it is a centralized solution, Etherscan provides an easily accessible list of transactions for an address.

    - **openRPC** / **openYP** - We use these two simple scripts to open web pages with much needed reference materials such as the RPC specifications and the Yellow Paper.
