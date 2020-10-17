## Libs

TrueBlocks is primarily a software library that gives quick access to better articulated data pulled from an Ethereum node. There are two fundamental libraries and three support libraries that build upon these fundamentals to provide all the capabilities one needs to build amazing applications using the Ethereum data.

#### Fundamentals

- [**The utillib library.**](utillib/README.md) A library providing basic services such as string utilities, concurrency-protected storage of binary files, time and date management, file system dependencies, etc.

- [**The etherlib library.**](etherlib/README.md) The core of the core of the system, this library provides access to the blockchain blocks, transactions, receipts, and logs as well as providing the interface to your Ethereum node's RPC interface. There are also classes and functions needed to read, store and manipulate ABI files.

#### Supporting

In addition to the above core libraries, TrueBlocks provides three more libraries, built upon the code, that work in support of TrueBlocks [monitors](../monitors/README.md):

- [**The acctlib library.**](acctlib/README.md) A library that allows for the building and manipulation of k-ary trees of Ethereum account data.

- [**The tokenlib library.**](tokenlib/README.md) A code library in support of parsing / supporting access to Ethereum transactional data related to ERC20 tokens.

- [**The walletlib library.**](walletlib/README.md) A code library in support of parsing / supporting access to Ethereum transactional data related to more popular multi-sig wallets.
