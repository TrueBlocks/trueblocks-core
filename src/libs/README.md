# QuickBlocks Libraries

The core of QuickBlocks is five libraries as follows:

1. [utillib](utillib) - a library to provide basic services such as string utilities, concurrency-protected storage of binary files, time and date management, file system dependencies, etc.
2. [abilib](abilib) - classes and functions needed to read, store and manipulate abi files.
3. [etherlib](etherlib) - the core of the core of the system, this library provides access to the blockchain blocks, transactions, receipts, and logs as well as providing the interface to the node's RPC interface
4. [tokenlib](tokenlib) - the is basically a 'default' monitor library that provides support for the ERC 20 token interface. By providing this as a base library, we allows end users to parse an important part of their smart contract's interactions without having to create a custom monitor parsing library. ERC 20 token support comes for free
5. [walletlib](walletlib) - in the same way we provide support for ERC 20 tokens, we provide support for the basic wallet functions. This allows the per-smart-contract code to be less complicated and provides support for some common functions without the need to create a customized library.
