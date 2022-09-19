## [{NAME}]

The `[{NAME}]` application creates TrueBlocks' chunked index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins each chunk of the index to IPFS.

`[{NAME}]` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. You may start and stop `[{NAME}]` as needed, but doing so means the scraper will not be keeping up with the front of teh blockchain. The next time it starts, it will have to catch up to the chain, a process that may take several hours depending on how long ago it was last run. See the section below and the "Papers" section of our website for more information on how the scraping process works and prerequisites for it proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On some machines, or when running against some EVM node software, the scraper may overburden the hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin` each new chunk to IPFS which naturally shards the database among all users. By default, pinning is against a locally running IPFS node, but the `--remote` option allows pinning to an IPFS pinning service such as Pinata or Estuary.

[{USAGE}][{CONFIG}]

### further information

Each time `[{NAME}]` runs, it begins at the last block it completed processing (plus one). With each pass, the scraper descends as deeply as is possible into each block's data. (This is why TrueBlocks requires a `--tracing` node.) As the scraper encounters appearances of address in the block's data, it adds those appearance to a growing index. Periodically (after processing the the block that contains the 2,000,000th appearance), the system consolidates an **index chunk**.

An **index chunk** is a portion of the index containing approximately 2,000,000 records (although, this number is adjustable for different chains). As part of the consolidation, the scraper creates a Bloom filter representing the set membership in the associated index portion. The Bloom filters are an order of magnitude smaller than the index chunks. The system then pushes both the index chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: We all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software) may subsequently download the Bloom filters, query them to determine which **index chunks** need to be downloaded, and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of resource requirement on the end user's machine.

Recently, we enabled the ability for the end user to pin these downloaded index chunks and blooms on their own machines. The user needs the data for the software to operate--sharing requires minimal effort and makes the data available to other people. Everyone is better off. A naturally-occuring network effect.

### prerequisites

`[{NAME}]` works with any EVM-based blockchain, but does not currently work without a "tracing, archive" RPC endpoint. The Erigon blockchain node, given its minimal disc footprint for an archive node and its support of the required `trace_` endpoint routines, is highly recommended.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for more information about running the scraper and building and sharing the index of appearances.

[{FOOTER}]
