---
title: "Admin"
description: ""
lead: ""
date: 2021-11-07T22:22:20
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
  -p, --port string   specify the server's port (:8080 default)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - To start API open terminal window and run chifra serve.
  - See the API documentation for more information.
```

**Source code**: [`serve.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/cmd/serve.go)

## chifra scrape

The `chifra scrape` application creates TrueBlocks' index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. It is possible to start and stop `chifra scrape` as needed, but doing so means the scraper needs to catch up to the front of the chain, a process that may take some time depending on how frequently the scraper is run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

The scraper can scrape either the index only, previously created monitors only, both, or neither. If you specify `none`, timestamps will be scraped but nothing else. If you're scraping monitors, you may tell the system to cache traces and transactions. This will speed up access, but take addition hard drive space. You may also adjust the speed of operation on different machines with the `--sleep` and `--block_cnt` options. Finally, you may choose to optionally `--pin` each new chunk to IPFS.

```[plaintext]
Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (indexer is default)
	One or more of [ indexer | monitors | both ]

Flags:
  -a, --action string    command to apply to the specified scrape
                         One of [ toggle | run | restart | pause | quit ]
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -p, --pin              pin chunks (and blooms) to IPFS as they are created (requires pinning service)
  -u, --publish          after pinning the chunk, publish it to UnchainedIndex
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - if no mode is presented, chifra scrape indexer --action run is assumed.
  - the --pin and --publish options require an API to the pinning service.
  - the --n_* related options allow you to tune the scrapers.
```

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed (plus one) and descends as deeply as it can into the block's data. (This is why we need a `--tracing` node.) As address appearances are encountered, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records. As part of the consolidation, the scraper creates a Bloom filter representing the chunk. The Bloom filters are an order of magnitude or more smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: we all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software, for that matter) subsequently downloads the Bloom filters, queries them to determine which **chunks** need to be downloaded to the user's machine and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of data on the end user's machine.

In future versions of the software, we will pin these shared chunks and blooms on end user's machines. They need the data for the software to operate and sharing it makes all users better off. A naturally-born network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software works without an `--archive` node, but it works significantly better with one. An additional requirement for the software to work properly is an RPC that provides OpenEthereum's `trace_` routines. We've tested with OpenEthereum and Erigon.

Please see [this article](.) for more information about running the scraper and building and sharing the index of appearances.


**Source code**: [`apps/blockScrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/blockScrape)

## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks [flags] <block> [block...]

Arguments:
  blocks - an optional list of blocks to process

Flags:
  -l, --list             list the bloom and index hashes from local cache or IPFS
  -c, --check            check the validity of the chunk or bloom
  -e, --extract string   show some or all of the contents of the chunk or bloom filters
                         One of [ header | addr_table | app_table | chunks | blooms ]
  -s, --stats            for the --list option only, display statistics about each chunk or bloom
  -a, --save             for the --extract option only, save the entire chunk to a similarly named file as well as display

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - Only a single block in a given chunk needs to be supplied.
```

**Source code**: [`apps/chunkMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chunkMan)

## chifra init

When invoked, `chifra init` looks at a smart contract called **The Unchained Index** ([0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd](https://etherscan.io/address/0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd)). From this smart contract, it extracts a data item called `manifestHash`. The `manifestHash` is an IPFS hash that points to a file (a manifest) that contains every previously pinned Bloom filter and index chunk. TrueBlocks periodically publishes the manifest's hash to the smart contract. This makes the entire index both available for our software to use and impossible for us to withhold. Both of these aspects of the manifest are included by design.

If you stop `chifra init` before it finishes, it will pick up against where it left off the next time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will start where `init` finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all   in addition to Bloom filters, download full index chunks

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - chifra init is an alias for the chifra pins --init command.
  - See chifra pins --help for more information.
```

**Source code**: [`init.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/cmd/init.go)

## chifra pins

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage pinned index of appearances and associated blooms.

Usage:
  chifra pins [flags]

Flags:
  -l, --list          list the bloom and index hashes from local cache or IPFS
  -i, --init          download the blooms or index chunks from IPFS
  -a, --all           in addition to Bloom filters, download full index chunks
  -S, --share         share downloaded data by pinning it to IPFS (the IPFS daemon must be running)
  -s, --sleep float   throttle requests by this many seconds (default 0.25)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.
```

**Source code**: [`pins.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/cmd/pins.go)

