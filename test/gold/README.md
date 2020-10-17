# tests

The remainder of the list are useful for testing, experimentation, and to teach various simple concepts used elsewhere in the system.

6. [abiTest](abiTest) - This tool localizes the task of encoding and decoding the function and event signatures from an ABI. If you're looking to understand how to encode function or event signatures, study this code. It's very simple.

7. [speedTest](speedTest) - This code performs a series of speed tests to make sure we maintain quick access to the data as we develop the software.

8. [acctTree](acctTree) - This simple demonstration code builds a 16-way radix-tree of accounts, accumulating each account's first block, most recent block, and 'naive' balance. The 'naive' balance can be checked against the balance stored for that account in the node. If they differ, the account has been the recipient of an internal transaction.

9. [sha3](sha3) - A simple demonstration of using the node to perform sha3 calculations.

10. [bytesUsed](bytesUsed) - A very simple tool to keep track of how much disk space the TrueBlocks system is consuming.
