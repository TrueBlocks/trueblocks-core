## Libs

QuickBlocks is primarily a software library that gives quick access to better articulated data pulled from an Ethereum node.

There are three fundamental libraries, as described first, and three libraries that build upon those fundamentals to provide all the capabilities we (and you) need to build amazing applications using the Ethereum data.

- **[The utillib library](utillib/README.md)** - a library providing basic services such as string utilities, concurrency-protected storage of binary files, time and date management, file system dependencies, etc.

- **[The abilib library](abilib/README.md)** - classes and functions needed to read, store and manipulate ABI files. This library supports much of the functionality in the [grabABI](../apps/grabABI/README.md) tool.

- **[The etherlib library](etherlib/README.md)** - the core of the core of the system, this library provides access to the blockchain blocks, transactions, receipts, and logs as well as providing the interface to your Ethereum node's RPC interface

In addition to the above three core libraries, QuickBlocks provides another few libraries that work in support of QuickBlocks [monitors](../monitors/README.md):

- **[The acctlib library](acctlib/README.md)** - xxx

- **[The tokenlib library](tokenlib/README.md)** - xxx

- **[The walletlib library](walletlib/README.md)** - xxx

