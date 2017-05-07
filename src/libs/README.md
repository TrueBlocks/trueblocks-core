# Libs

**Note:**

Currently we provide only the header files and static .a libraries. As these libraries are better documented and more well-tested, we will release this code further.

<hr>

The core of QuickBlocks is the following libraries:

1. [utillib](utillib) - a library to provide basic services such as string utilities, concurrency-protected storage of binary files, time and date management, file system dependencies, etc.
2. [abilib](abilib) - classes and functions needed to read, store and manipulate abi files.
3. [etherlib](etherlib) - the core of the core of the system, this library provides access to the blockchain blocks, transactions, receipts, and logs as well as providing the interface to the node's RPC interface
