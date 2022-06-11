---
title: "Admin"
description: ""
lead: ""
date: 2022-06-10T16:43:36
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
  -x, --fmt string    export format, one of [none|json*|txt|csv|api]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - To start API open terminal window and run chifra serve.
  - See the API documentation for more information.
```

**Source code**: [`server`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/server)

## chifra scrape

The `chifra scrape` application creates TrueBlocks' index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. You may start and stop `chifra scrape` as needed, but doing so means the scraper will have to catch up to the front of the chain the next time it runs, a process that may take several hours depending on how long ago it was last run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On some machines, or when running against some EVM node software, you may overburden the hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin` each new chunk to IPFS which naturally shards the database among all users.

```[plaintext]
Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.

Usage:
  chifra scrape <mode> [mode...] [flags]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ run | stop ]

Flags:
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -p, --pin              pin chunks (and blooms) to IPFS as they are created (requires pinning service)
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed processing (plus one). With each pass, it descends as deeply as it can into the block's data. (This is why the indexer requires a `--tracing` node.) As addresses appear in the blocks, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records (although, this number is adjustable for different chains). As part of the consolidation, the scraper creates a Bloom filter representing the set membership in the chunk. The Bloom filters are an order of magnitude (or more) smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: We all, every one of us, need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software) may subsequently download the Bloom filters, query them to determine which **chunks** need to be downloaded, and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of resource requirement on the end user's machine.

In future versions of the software, we will pin these downloaded chunks and blooms on end user's machines. The user needs the data for the software to operate and sharing it makes all everyone better off. A naturally-occuring network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software *works* without an `archive` node, but it works significantly better with one. An additional requirement is an RPC that provides OpenEthereum's `trace_` routines. We suggest strongly that you use Erigon for many reasons.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for more information about running the scraper and building and sharing the index of appearances.


**Source code**: [`internal/scrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/scrape)

## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of chunk info to retrieve (required)
	One of [ stats | pins | blooms | index | addresses | appearances ]
  blocks - optional list of blocks to intersect with chunk ranges
  addrs - one or more addresses to use with --belongs option (see note)

Flags:
  -c, --check        depends on mode, checks for internal consistency of the data type
  -b, --belongs      checks if the given address appears in the given chunk
  -d, --details      for the --addresses option only, display appearance records as well as address records
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - Only a single block in a given chunk needs to be supplied for a match.
  - The --belongs option is only available with the addresses or blooms mode.
  - The --belongs option requires both an address and a block identifier.
  - You may only specifiy an address when using the --belongs option.
```

**Source code**: [`internal/chunks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

## chifra init

When invoked, `chifra init` looks at a smart contract called **The Unchained Index** ([0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd](https://etherscan.io/address/0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd)). From this smart contract, it extracts a data item called `manifestHash`. The `manifestHash` is an IPFS hash that points to a file (a manifest) that contains every previously pinned Bloom filter and index chunk. TrueBlocks periodically publishes the manifest's hash to the smart contract. This makes the entire index both available for our software to use and impossible for us to withhold. Both of these aspects of the manifest are included by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will start where `init` finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all          in addition to Bloom filters, download full index chunks
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
```

**Source code**: [`internal/init`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/init)

