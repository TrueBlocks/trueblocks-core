---
title: "Admin"
description: ""
lead: ""
date: 2022-05-02T07:04:38
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

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. It is possible to start and stop `chifra scrape` as needed, but doing so means the scraper needs to catch up to the front of the chain, a process that may take some time depending on how frequently the scraper is run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

The scraper can scrape either the index only, previously created monitors only, both, or neither. If you specify `none`, timestamps will be scraped but nothing else. If you're scraping monitors, you may tell the system to cache traces and transactions. This will speed up access, but take addition hard drive space. You may also adjust the speed of operation on different machines with the `--sleep` and `--block_cnt` options. Finally, you may choose to optionally `--pin` each new chunk to IPFS.

```[plaintext]
Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ indexer | monitors | both ]

Flags:
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -p, --pin              pin chunks (and blooms) to IPFS as they are created (requires pinning service)
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed (plus one) and descends as deeply as it can into the block's data. (This is why we need a `--tracing` node.) As address appearances are encountered, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records. As part of the consolidation, the scraper creates a Bloom filter representing the chunk. The Bloom filters are an order of magnitude or more smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: we all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software, for that matter) subsequently downloads the Bloom filters, queries them to determine which **chunks** need to be downloaded to the user's machine and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of data on the end user's machine.

In future versions of the software, we will pin these shared chunks and blooms on end user's machines. They need the data for the software to operate and sharing it makes all users better off. A naturally-born network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software works without an `--archive` node, but it works significantly better with one. An additional requirement for the software to work properly is an RPC that provides OpenEthereum's `trace_` routines. We've tested with OpenEthereum and Erigon.

Please see [this article](.) for more information about running the scraper and building and sharing the index of appearances.


**Source code**: [`internal/scrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/scrape)

## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks [flags] <block> [block...]

Arguments:
  blocks - optional list of blocks to intersect with chunk ranges

Flags:
  -e, --extract string   show some or all of the contents of the chunk or bloom filters
                         One of [ stats | pins | blooms | index | header | addresses | appearances ]
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen

Notes:
  - Only a single block in a given chunk needs to be supplied.
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

