---
title: "Admin"
description: ""
lead: ""
date: 2022-10-08T19:54:08
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1700
toc: true
---

The Admin component allows you to query the status of the TrueBlocks system.
You can query the status; query for information about TrueBlocks caches;
control the creation, sharing, and pinning of the TrueBlocks index of appearances;
and even serve the data through an API.

[See the API documentation for all information about using the API](/api).

## chifra serve

`chifra serve` delivers a JSON API for each of the `chifra` commands along with each of its options.
It does this through `flame` server, which is written in Go.

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options.

```[plaintext]
Purpose:
  Serve the TrueBlocks API using the flame server.

Usage:
  chifra serve [flags]

Flags:
  -p, --port string   specify the server's port (default ":8080")
  -x, --fmt string    export format, one of [none|json*|txt|csv]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - To start API open terminal window and run chifra serve.
  - See the API documentation for more information.
```

**Source code**: [`server`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/server)

## chifra scrape

The `chifra scrape` application creates TrueBlocks' chunked index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins each chunk of the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. You may start and stop `chifra scrape` as needed, but doing so means the scraper will not be keeping up with the front of teh blockchain. The next time it starts, it will have to catch up to the chain, a process that may take several hours depending on how long ago it was last run. See the section below and the "Papers" section of our website for more information on how the scraping process works and prerequisites for it proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On some machines, or when running against some EVM node software, the scraper may overburden the hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin` each new chunk to IPFS which naturally shards the database among all users. By default, pinning is against a locally running IPFS node, but the `--remote` option allows pinning to an IPFS pinning service such as Pinata or Estuary.

```[plaintext]
Purpose:
  Scan the chain and update the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags]

Flags:
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -i, --pin              pin new chunks (requires locally-running IPFS daemon or --remote)
  -m, --remote           pin new chunks to the gateway (requires pinning service keys)
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -x, --fmt string       export format, one of [none|json*|txt|csv]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

### configuration

Each of the following additional configurable command line options are available.

**Configuration file:** `$CONFIG/$CHAIN/blockScrape.toml`  
**Configuration group:** `[settings]`  

| Item               | Type         | Default      | Description / Default |
| ------------------ | ------------ | ------------ | --------- |
| apps&lowbar;per&lowbar;chunk | uint64       | 200000       | the number of appearances to build into a chunk before consolidating it |
| snap&lowbar;to&lowbar;grid | uint64       | 100000       | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| first&lowbar;snap  | uint64       | 0            | the first block at which snap_to_grid is enabled |
| unripe&lowbar;dist | uint64       | 28           | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe |
| channel&lowbar;count | uint64       | 20           | number of concurrent processing channels |
| allow&lowbar;missing | bool         | true         | do not report errors for blockchains that contain blocks with zero addresses |


These items may be set in three ways, each overridding the preceeding method:

-- in the above configuration file under the `[settings]` group,  
-- in the environment by exporting the configuration item as UPPER&lowbar;CASE, without underbars, and prepended with TB_SETTINGS&lowbar;, or  
-- on the command line using the configuration item with leading dashes (i.e., `--name`).  

### further information

Each time `chifra scrape` runs, it begins at the last block it completed processing (plus one). With each pass, the scraper descends as deeply as is possible into each block's data. (This is why TrueBlocks requires a `--tracing` node.) As the scraper encounters appearances of address in the block's data, it adds those appearance to a growing index. Periodically (after processing the the block that contains the 2,000,000th appearance), the system consolidates an **index chunk**.

An **index chunk** is a portion of the index containing approximately 2,000,000 records (although, this number is adjustable for different chains). As part of the consolidation, the scraper creates a Bloom filter representing the set membership in the associated index portion. The Bloom filters are an order of magnitude smaller than the index chunks. The system then pushes both the index chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: We all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software) may subsequently download the Bloom filters, query them to determine which **index chunks** need to be downloaded, and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of resource requirement on the end user's machine.

Recently, we enabled the ability for the end user to pin these downloaded index chunks and blooms on their own machines. The user needs the data for the software to operate--sharing requires minimal effort and makes the data available to other people. Everyone is better off. A naturally-occuring network effect.

### prerequisites

`chifra scrape` works with any EVM-based blockchain, but does not currently work without a "tracing, archive" RPC endpoint. The Erigon blockchain node, given its minimal disc footprint for an archive node and its support of the required `trace_` endpoint routines, is highly recommended.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for more information about running the scraper and building and sharing the index of appearances.


**Source code**: [`internal/scrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/scrape)

## chifra chunks

The chifra chunks routine provides tools for interacting with, checking the validity of,
cleaning up, and analyizing the Unchained Index. It provides options to list pins,
the Manifest, summary data on the index, Bloom filters, addresses, and appearances.
While still in its early stages, this tool will eventually allow users to clean
their local index, clean their remote index, study the indexes, etc. Stay tuned.

```[plaintext]
Purpose:
  Manage, investigate, and display the Unchained Index.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of data to process (required)
	One of [ status | manifest | index | blooms | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges

Flags:
  -c, --check             check the manifest, index, or blooms for internal consistency
  -i, --pin               pin the manifest or each index chunk and bloom
  -p, --publish           publish the manifest to the Unchained Index smart contract
  -n, --truncate uint     truncate the entire index at this block (requires a block identifier)
  -m, --remote            prior to processing, retreive the manifest from the Unchained Index smart contract
  -b, --belongs strings   in index mode only, checks the address(es) for inclusion in the given index chunk
  -s, --sleep float       for --remote pinning only, seconds to sleep between API calls
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose (increase detail with --log_level)
  -h, --help              display this help screen

Notes:
  - Mode determines which type of data to display or process.
  - Certain options are only available in certain modes.
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - The --truncate option updates data, but does not --pin or --publish.
  - You may combine the --pin and --publish options.
  - The --belongs option is only available in the index mode.
```

**Source code**: [`internal/chunks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

## chifra init

When invoked, `chifra init` reads a value from a smart contract called **The Unchained Index**
([0x0c316b7042b419d07d343f2f4f5bd54ff731183d](https://etherscan.io/address/0x0c316b7042b419d07d343f2f4f5bd54ff731183d)).

This value (`manifestHashMap`) is an IPFS hash pointing to a pinned file (called the Manifest) that
contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom
filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract.
This makes the index available for our software to use and impossible for us to withhold. Both of these
aspects of the manifest are by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will start where `init` finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all           in addition to Bloom filters, download full index chunks
  -s, --sleep float   seconds to sleep between downloads
  -x, --fmt string    export format, one of [none|json*|txt|csv]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
```

**Source code**: [`internal/init`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/init)

