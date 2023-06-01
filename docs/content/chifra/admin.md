---
title: "Admin"
description: ""
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
aliases:
 - "/docs/chifra/admin"
menu:
  chifra:
    parent: commands
weight: 1600
toc: true
---
<!-- markdownlint-disable MD033 MD036 MD041 -->
The Admin gourp of commands allows you to query the status of the TrueBlocks system and
manage various apects including the Unchained Index. You may query the status; query for information
about TrueBlocks caches; control the creation, sharing, and pinning of the Unchained Index; and even
serve the data through an API.

See [the API documentation](/api) for all information about using the API.

To the right is a list of commands in this group. Click on a command to see its full documentation.
## chifra config

<!-- markdownlint-disable MD041 -->
The `chifra config` program allows you to manage the various TrueBlocks caches. You may list all of the
caches, some of the cache, or even individual caches either in terse or full detail. The cache of
interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as
well as other data including blockchain data, and address monitors.

```[plaintext]
Purpose:
  Report on and edit the configuration of the TrueBlocks system.

Usage:
  chifra config <mode> [flags]

Arguments:
  mode - either show or edit the configuration
	One of [ show | edit ]

Flags:
  -a, --paths        show the configuration paths for the system
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen
```

Data models produced by this tool:

- [chain](/data-model/admin/#chain)

Links:

- [api docs](/api/#operation/admin-config)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/config)

## chifra status


```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
	One or more of [ index | blooms | blocks | txs | traces | monitors | names | abis | recons | slurps | staging | unripe | maps | some | all ]

Flags:
  -c, --first_record uint   the first record to process (default 1)
  -e, --max_records uint    the maximum number of records to process (default 10000)
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose (increase detail with --log_level)
  -h, --help                display this help screen

Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
```

Data models produced by this tool:

- [status](/data-model/admin/#status)
- [cacheitem](/data-model/admin/#cacheitem)
- [chain](/data-model/admin/#chain)

Links:

- [api docs](/api/#operation/admin-status)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/status)

## chifra daemon

<!-- markdownlint-disable MD041 -->
`chifra daemon` manages chifra's long-running processes include its JSON API server. Each of the
`chifra` commands along with all of its options, are provided not only by the command line, but
also the API server. We call this process the `flame` server, which is written in Go.

In the future, this daemon may also manage other long-running processes.

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line.
See below for an example of converting command line options to a call to the API. There's a
one-to-one correspondence between the command line tools and options and the API routes and
their options.

```[plaintext]
Purpose:
  Initalize and control long-running processes such as the API and the scrapers.

Usage:
  chifra daemon [flags]

Aliases:
  daemon, serve

Flags:
  -p, --port string     specify the server's port (default ":8080")
  -a, --api string      instruct the node to start the API server
                        One of [ off | on ] (default "on")
  -s, --scrape string   start the scraper, initialize it with either just blooms or entire index, generate for new blocks
                        One of [ off | blooms | index ]
  -m, --monitor         instruct the node to start the monitors tool
  -g, --grpc            run gRPC server to serve names
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - To start API open terminal window and run chifra daemon.
  - See the API documentation (https://trueblocks.io/api) for more information.
```

Data models produced by this tool:

- none

Links:

- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/daemon)

## chifra scrape

<!-- markdownlint-disable MD041 -->
The `chifra scrape` application creates TrueBlocks' chunked index of address appearances -- the
fundamental data structure of the entire system. It also, optionally, pins each chunk of the index
to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal
multiplexer such as `tmux`. You may start and stop `chifra scrape` as needed, but doing so means the
scraper will not be keeping up with the front of the blockchain. The next time it starts, it will
have to catch up to the chain, a process that may take several hours depending on how long ago it
was last run. See the section below and the "Papers" section of our website for more information
on how the scraping process works and prerequisites for it proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On
some machines, or when running against some EVM node software, the scraper may overburden the
hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin`
each new chunk to IPFS which naturally shards the database among all users. By default, pinning
is against a locally running IPFS node, but the `--remote` option allows pinning to an IPFS
pinning service such as Pinata or Estuary.

```[plaintext]
Purpose:
  Scan the chain and update the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags]

Flags:
  -n, --block_cnt uint     maximum number of blocks to process per pass (default 2000)
  -i, --pin                pin new chunks (requires locally-running IPFS daemon or --remote)
  -r, --remote             pin new chunks to the gateway (requires pinning service keys)
  -s, --sleep float        seconds to sleep between scraper passes (default 14)
  -l, --start_block uint   first block to visit when scraping (snapped back to most recent snap_to_grid mark)
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose (increase detail with --log_level)
  -h, --help               display this help screen
```

Data models produced by this tool:

- [manifest](/data-model/admin/#manifest)
- [chunkrecord](/data-model/admin/#chunkrecord)

Links:

- [api docs](/api/#operation/admin-scrape)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/scrape)

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

<!-- markdownlint-disable MD041 -->
### further information

Each time `chifra scrape` runs, it begins at the last block it completed processing (plus one). With
each pass, the scraper descends as deeply as is possible into each block's data. (This is why
TrueBlocks requires a `--tracing` node.) As the scraper encounters appearances of address in the
block's data, it adds those appearance to a growing index. Periodically (after processing the the
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

## chifra chunks

<!-- markdownlint-disable MD041 -->
The chifra chunks routine provides tools for interacting with, checking the validity of, cleaning up,
and analyizing the Unchained Index. It provides options to list pins, the Manifest, summary data
on the index, Bloom filters, addresses, and appearances. While still in its early stages, this
tool will eventually allow users to clean their local index, clean their remote index, study
the indexes, etc. Stay tuned.

```[plaintext]
Purpose:
  Manage, investigate, and display the Unchained Index.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of data to process (required)
	One of [ manifest | index | blooms | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges

Flags:
  -c, --check              check the manifest, index, or blooms for internal consistency
  -i, --pin                pin the manifest or each index chunk and bloom
  -p, --publish            publish the manifest to the Unchained Index smart contract
  -r, --remote             prior to processing, retreive the manifest from the Unchained Index smart contract
  -b, --belongs strings    in index mode only, checks the address(es) for inclusion in the given index chunk
  -F, --first_block uint   first block to process (inclusive)
  -L, --last_block uint    last block to process (inclusive)
  -m, --max_addrs uint     the max number of addresses to process in a given chunk
  -d, --deep               if true, dig more deeply during checking (manifest only)
  -s, --sleep float        for --remote pinning only, seconds to sleep between API calls
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose (increase detail with --log_level)
  -h, --help               display this help screen

Notes:
  - Mode determines which type of data to display or process.
  - Certain options are only available in certain modes.
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - The --truncate option updates the manifest and removes local data, but does not alter remote pins.
  - The --belongs option is only available in the index mode.
  - The --first_block and --last_block options apply only to addresses, appearances, and index --belongs mode.
  - The --pin option requires a locally running IPFS node or a pinning service API key.
  - The --publish option requires a private key.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [manifest](/data-model/admin/#manifest)
- [chunkrecord](/data-model/admin/#chunkrecord)
- [chunkindex](/data-model/admin/#chunkindex)
- [chunkbloom](/data-model/admin/#chunkbloom)
- [chunkaddress](/data-model/admin/#chunkaddress)
- [chunkstats](/data-model/admin/#chunkstats)
- [reportcheck](/data-model/admin/#reportcheck)
- [chunkpinreport](/data-model/admin/#chunkpinreport)

Links:

- [api docs](/api/#operation/admin-chunks)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

## chifra init

<!-- markdownlint-disable MD041 -->
When invoked, `chifra init` reads a value from a smart contract called **The Unchained Index**
([0x0c316b7042b419d07d343f2f4f5bd54ff731183d](https://etherscan.io/address/0x0c316b7042b419d07d343f2f4f5bd54ff731183d)).

This value (`manifestHashMap`) is an IPFS hash pointing to a pinned file (called the Manifest) that
contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom
filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract.
This makes the index available for our software to use and impossible for us to withhold. Both of
these aspects of the manifest are by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next
time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not
previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's
satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will
start where `init` finished. This means that only the blooms will be stored on your hard drive.
Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete
chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading the Unchained Index from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all                in addition to Bloom filters, download full index chunks (recommended)
  -d, --dry_run            display the results of the download without actually downloading
  -F, --first_block uint   do not download any chunks earlier than this block
  -s, --sleep float        seconds to sleep between downloads
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose (increase detail with --log_level)
  -h, --help               display this help screen

Notes:
  - If run with no options, this tool will download or freshen only the Bloom filters.
  - The --first_block option will fall back to the start of the containing chunk.
  - You may re-run the tool as often as you wish. It will repair or freshen the index.
```

Data models produced by this tool:

- [manifest](/data-model/admin/#manifest)
- [chunkrecord](/data-model/admin/#chunkrecord)

Links:

- [api docs](/api/#operation/admin-init)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/init)

