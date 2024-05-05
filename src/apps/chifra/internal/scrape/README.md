## chifra scrape

The `chifra scrape` application creates TrueBlocks' chunked index of address appearances -- the
fundamental data structure of the entire system. It also, optionally, pins each chunk of the index
to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal
multiplexer such as `tmux`. You may start and stop `chifra scrape` as needed, but doing so means the
scraper will not be keeping up with the front of the blockchain. The next time it starts, it will
have to catch up to the chain, a process that may take several hours depending on how long ago it
was last run. See the section below and the "Papers" section of our website for more information
on how the scraping process works and prerequisites for its proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On
some machines, or when running against some EVM node software, the scraper may overburden the
hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin`
each new chunk to IPFS which naturally shards the database among all users. By default, pinning
is against a locally running IPFS node, but the `--remote` option allows pinning to an IPFS
pinning service such as Pinata.

```[plaintext]
Purpose:
  Scan the chain and update the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags]

Flags:
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -l, --touch uint       first block to visit when scraping (snapped back to most recent snap_to_grid mark)
  -v, --verbose          enable verbose output
  -h, --help             display this help screen

Notes:
  - The --touch option may only be used for blocks after the latest scraped block (if any). It will be snapped back to the latest snap_to block.
```

Data models produced by this tool:

- [chunkrecord](/data-model/admin/#chunkrecord)
- [manifest](/data-model/admin/#manifest)

### configuration

Each of the following additional configurable command line options are available.

**Configuration file:** `trueBlocks.toml`  
**Configuration group:** `[scrape.<chain>]`

| Item         | Type   | Default | Description / Default                                                                                                    |
| ------------ | ------ | ------- | ------------------------------------------------------------------------------------------------------------------------ |
| appsPerChunk | uint64 | 2000000 | the number of appearances to build into a chunk before consolidating it                                                  |
| snapToGrid   | blknum | 250000  | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| firstSnap    | blknum | 2000000 | the first block at which snap_to_grid is enabled                                                                         |
| unripeDist   | blknum | 28      | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe                |
| channelCount | uint64 | 20      | number of concurrent processing channels                                                                                 |
| allowMissing | bool   | false   | do not report errors for blockchains that contain blocks with zero addresses                                             |

Note that for Ethereum mainnet, the default values for appsPerChunk and firstSnap are 2,000,000 and 2,300,000 respectively. See the specification for a justification of these values.

These items may be set in three ways, each overriding the preceding method:

-- in the above configuration file under the `[scrape.<chain>]` group,  
-- in the environment by exporting the configuration item as upper case (with underbars removed) and prepended with (TB underbar SCRAPE underbar CHAIN) with the underbars included, or  
-- on the command line using the configuration item with leading dashes and in snake case (i.e., `--snake_case`).

### further information

Each time `chifra scrape` runs, it begins at the last block it completed processing (plus one). With
each pass, the scraper descends as deeply as is possible into each block's data. (This is why
TrueBlocks requires a `--tracing` node.) As the scraper encounters appearances of address in the
block's data, it adds those appearances to a growing index. Periodically (after processing the
block that contains the 2,000,000th appearance), the system consolidates an **index chunk**.

An **index chunk** is a portion of the index containing approximately 2,000,000 records (although,
this number is adjustable for different chains). As part of the consolidation, the scraper creates
a Bloom filter representing the set membership in the associated index portion. The Bloom filters
are an order of magnitude smaller than the index chunks. The system then pushes both the index
chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable
index of appearances that can be used not only by TrueBlocks, but any member of the community who
needs it. (Hint: We all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other
software) may subsequently download the Bloom filters, query them to determine which **index
chunks** need to be downloaded, and thereby build a historical list of transactions for a given
address. This is accomplished while imposing a minimum amount of resource requirement on the end
user's machine.

Recently, we enabled the ability for the end user to pin these downloaded index chunks and blooms
on their own machines. The user needs the data for the software to operate--sharing requires
minimal effort and makes the data available to other people. Everyone is better off. A
naturally-occuring network effect.

### prerequisites

`chifra scrape` works with any EVM-based blockchain, but does not currently work without a "tracing,
archive" RPC endpoint. The Erigon blockchain node, given its minimal disc footprint for an archive
node and its support of the required `trace_` endpoint routines, is highly recommended.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for
more information about running the scraper and building and sharing the index of appearances.

### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
