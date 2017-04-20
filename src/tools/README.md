# quickBlocks Tools

This folder contains various tools that help in the build process of a QuickBlocks libraries, monitors, tools and applications as well as in various testing and scripting scenarios:

1. [grabABI](grabABI) - This is the basis of the `chafra` application. Given an Etheruem address, this tool first grabs the ABI from where ever it can find it (http://etherscan.io, ENS, etc.). It then parses the ABI and writes all the C++ necessary to build an Ethereum address monitor. `chafra` uses this tool extensively.

2. [makeClass](makeClass) - This tool is used by `grabABI` to write the actual C++ code generated during the `chafra` process. Many of the source files found in `etherlib` are also generated automatically using this tool.

3. [cacheMan](cacheMan) - This tool allows one to explore the a monitor's binary cache data file. It preforms various checks on the data. The tool provides options for truncating the cache and/or merging two caches into one which may be useful in certain circumstances.

4. [ethname](ethname) - This simple tool provides a rudimentary ENS-like service attaching Ethereum address to names and visa versa.

5. [whenBlock](whenBlock) - This surprisingly handy tool allows you to enter either a block number and it returns the date of that block, or a date and it returns the block just prior to the date. It makes exploring and analyzing the blockchain much easier because you don't have to remember block numbers.

The remainder of the list are useful for testing, experimentation, and to teach various simple concepts used elsewhere in the system.

6. [abiTest](abiTest) - This tool localizes the task of encoding and decoding the function and event signatures from an ABI. If you're looking to understand how to encode function or event signatures, study this code. It's very simple.

7. [speedTest](speedTest) - This code performs a series of speed tests to make sure we maintain quick access to the data as we develop the software.

8. [acctTree](acctTree) - This simple demonstration code builds a 16-way radix-tree of accounts, accumulating each account's first block, most recent block, and 'naive' balance. The 'naive' balance can be checked against the balance stored for that account in the node. If they differ, the account has been the recipient of an internal transaction.

9. [sha3](sha3) - A simple demonstration of using the node to perform sha3 calculations.

10. [bytesUsed](bytesUsed) - A very simple tool to keep track of how much disk space the QuickBlocks system is consuming.

11. [scripts](scripts) - A collection of simple bash shell scripts that make working with the blockchain simpler.

Each of these tools is explained further in its own individual README.md files.
