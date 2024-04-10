---
title: "Admin"
description: "Control the scraper and build the index"
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
weight: 41000
toc: true
---

The Admin group of commands allows you to query the status of the TrueBlocks system and
manage various aspects including the Unchained Index. You may query the status; query for information
about TrueBlocks caches; control the creation, sharing, and pinning of the Unchained Index; and even
serve the data through an API.

See [the API documentation](/api) for all information about using the API.

To the right is a list of commands in this group. Click on a command to see its full documentation.

## chifra config

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
  -v, --verbose      enable verbose output
  -h, --help         display this help screen
```

Data models produced by this tool:

- [chain](/data-model/admin/#chain)

Links:

- [api docs](/api/#operation/admin-config)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/config)

## chifra status

The `chifra status` tool reports on the state (and size) of the various TrueBlocks local binary
caches. TrueBlocks produces nine difference caches: `abis`, `blocks`, `monitors`, `names`, `objs`,
`recons`, `slurps`, `traces`, `transactions`. In general practice, these caches may take up a
few GB of hard drive space, however, for very popular smart contract the size of the caches may
grow rather large. Keep an eye on it.

The `chifra status` program allows you to manage the various TrueBlocks caches. You may list all of the
caches, some of the cache, or even individual caches either in terse or full detail. The cache of
interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as
well as other data including blockchain data, and address monitors.

```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
	One or more of [ index | blooms | blocks | transactions | traces | logs | statements | results | state | tokens | monitors | names | abis | slurps | staging | unripe | maps | some | all ]

Flags:
  -d, --diagnose            same as the default but with additional diagnostics
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 10000)
  -a, --chains              include a list of chain configurations in the output
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
```

Data models produced by this tool:

- [cacheitem](/data-model/admin/#cacheitem)
- [chain](/data-model/admin/#chain)
- [status](/data-model/admin/#status)

Links:

- [api docs](/api/#operation/admin-status)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/status)

## chifra daemon

`chifra daemon` manages chifra's API server. Each of the `chifra` commands along with all of its options,
are provided not only by the command line, but also the API server. We call this process the
`flame` server, which is written in Go. `chifra serve` is an alias for the `chifra daemon` command.

In the future, this daemon may also manage other long-running processes such as `chifra scrape`
and `chifra monitors`, but for now, it's only managing the API server.

The `--grpc` option turns on a GRPC server that may speed up certain command such as `chifra names`,
although this option is experimental and therefore not recommended for production use.

If the default port for the API server is in use, you may change it with the `--port` option.

To get help for any command, please see the API documentation on our website. But, you may
also run `chifra --help` or `chifra <cmd> --help` on your command line to get help.

See below for an example of converting command line options to a call to the API. There's a
one-to-one correspondence between the command line tools and options and the API routes and
their options.

```[plaintext]
Purpose:
  Initialize and control long-running processes such as the API and the scrapers.

Usage:
  chifra daemon [flags]

Aliases:
  daemon, serve

Flags:
  -u, --url string   specify the API server's url and optionally its port (default "localhost:8080")
  -g, --grpc         run gRPC server to serve names
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - To start API open terminal window and run chifra daemon.
  - See the API documentation (https://trueblocks.io/api) for more information.
```

Data models produced by this tool:

- none

Links:

- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/daemon)

### further information

To convert the options for a command line tool to an API call, do the following:

1. Any `--snake_case` argument to the command line should be converted to `camelCase`. For example, `--no_header` on the command line should be sent as `&noHeader` to the API server.
2. Any `switch` on the command line, (i.e., options whose presence indicates `true` and whose absence indicates `false`) should be sent as a `boolean` to the API server. For example, `--no_header` on the command line should be sent as `&noHeader=true` to the API server. If the option is `fales`, you do not need to send it to the API server.
3. Positionals such as the addresses, topics, and four-bytes for `chifra export`, must be prepended with their positional name. For example, `chifra export <address> <topic>` should be sent as `&addrs=<address>&topics=<topic>` to the API server. For some commands (experiment) you may send more than one value for a positional with `%20` separating the entries or by sending multiple positionals (i.e., `&addrs=<address1>&addrs=<address2>`).

<hr />
<span style="size: -2; background-color: #febfc1; color: black; display: block; padding: 4px">
Chifra was built for the command line, a fact we purposefully take advantage of to ensure continued operation on small machines. As such, this tool is not intended to serve multiple end users in a cloud-based server environment. This is by design. Be forewarned.
</span>
<hr />

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

Links:

- [api docs](/api/#operation/admin-scrape)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/scrape)

### configuration

Each of the following additional configurable command line options are available.

**Configuration file:** `trueBlocks.toml`  
**Configuration group:** `[scrape.<chain>]`

| Item         | Type   | Default | Description / Default                                                                                                    |
| ------------ | ------ | ------- | ------------------------------------------------------------------------------------------------------------------------ |
| appsPerChunk | uint64 | 2000000 | the number of appearances to build into a chunk before consolidating it                                                  |
| snapToGrid   | uint64 | 250000  | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| firstSnap    | uint64 | 2000000 | the first block at which snap_to_grid is enabled                                                                         |
| unripeDist   | uint64 | 28      | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe                |
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

## chifra chunks

The `chifra chunks` routine provides tools for interacting with, checking the validity of, cleaning up,
and analyzing the Unchained Index. It provides options to list pins, the Manifest, summary data
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
	One of [ manifest | index | blooms | pins | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges

Flags:
  -c, --check              check the manifest, index, or blooms for internal consistency
  -i, --pin                pin the manifest or each index chunk and bloom
  -p, --publish            publish the manifest to the Unchained Index smart contract
  -r, --remote             prior to processing, retrieve the manifest from the Unchained Index smart contract
  -b, --belongs strings    in index mode only, checks the address(es) for inclusion in the given index chunk
  -F, --first_block uint   first block to process (inclusive)
  -L, --last_block uint    last block to process (inclusive)
  -m, --max_addrs uint     the max number of addresses to process in a given chunk
  -d, --deep               if true, dig more deeply during checking (manifest only)
  -e, --rewrite            for the --pin --deep mode only, writes the manifest back to the index folder (see notes)
  -U, --count              for the pins mode only, display only the count of records
  -s, --sleep float        for --remote pinning only, seconds to sleep between API calls
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose output
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
  - The --publisher option is ignored with the --publish option since the sender of the transaction is recorded as the publisher.
  - Without --rewrite, the manifest is written to the temporary cache. With it, the manifest is rewritten to the index folder.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [appearancetable](/data-model/accounts/#appearancetable)
- [chunkaddress](/data-model/admin/#chunkaddress)
- [chunkbloom](/data-model/admin/#chunkbloom)
- [chunkindex](/data-model/admin/#chunkindex)
- [chunkpinreport](/data-model/admin/#chunkpinreport)
- [chunkrecord](/data-model/admin/#chunkrecord)
- [chunkstats](/data-model/admin/#chunkstats)
- [ipfspin](/data-model/admin/#ipfspin)
- [manifest](/data-model/admin/#manifest)
- [reportcheck](/data-model/admin/#reportcheck)

Links:

- [api docs](/api/#operation/admin-chunks)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

## chifra init

When invoked, `chifra init` reads a value from a smart contract called **The Unchained Index**
(0x0c316b7042b419d07d343f2f4f5bd54ff731183d).

This value (`manifestHashMap`) is an IPFS hash pointing to a pinned file (called the Manifest) that
contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom
filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract.
This makes the index available for our software to use and impossible for us to withhold. Both of
these aspects of the manifest are by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next
time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not
previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's
satisfied.

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
  -v, --verbose            enable verbose output
  -h, --help               display this help screen

Notes:
  - If run with no options, this tool will download or freshen only the Bloom filters.
  - The --first_block option will fall back to the start of the containing chunk.
  - You may re-run the tool as often as you wish. It will repair or freshen the index.
```

Data models produced by this tool:

- [chunkrecord](/data-model/admin/#chunkrecord)
- [manifest](/data-model/admin/#manifest)

Links:

- [api docs](/api/#operation/admin-init)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/init)

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
