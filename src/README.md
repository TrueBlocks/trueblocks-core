# quickBlocks

The `src` folder holds all the source code for QuickBlocks.

Under the `src` folder, the `app` folder holds program source code for the primary programs that implement the system. This consists of the block chain scraper (`blockScraper`), the miniBlock indexer (`miniBlocks`), and various other command-line tools used to query the Ethereum node.

The `libs` folder holds code useful various code used across many applications and tools.

The `monitors` folder contains code that has specific meaning only to particular smart contracts. There are examples provided that 'monitor' a few popular smart contracts such as "The DAO", "SingularDTV" and the "ENS". This code is intended as an example of how to build your own QuickBlocks monitors for your own smart contracts.

The `tools` folder contains various tools that come in handy, for example, to manage the caches created by the `monitors`.
