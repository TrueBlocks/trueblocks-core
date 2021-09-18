---
title: "Admin"
description: ""
lead: ""
date: 2021-09-17T22:41:07
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
The Admin component allows you to query the status of the TrueBlocks system. You may query the status; query for information about TrueBlocks caches; control the creation, sharing, and pinning of the TrueBlocks index of appearances; and even serve the data through an API. See our [API documentation](/api) for more information.
## chifra status

The `chifra status` program allows you to manage the various TrueBlock caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called collections.

### usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on the status of the TrueBlocks system.

`Where:`

{{<td>}}
|          | Option                          | Description                                                                                                                        |
| -------- | ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
|          | modes                           | the type of status info to retrieve, one or more of<br/>[index, monitors, collections, names, abis, caches,<br/>some, all]         |
| &#8208;d | &#8208;&#8208;details           | include details about items found in monitors, slurps,<br/>abis, or price caches                                                   |
| &#8208;t | &#8208;&#8208;types &lt;val&gt; | for caches mode only, which type(s) of cache to report,<br/>one or more of [blocks, transactions, traces, slurps,<br/>prices, all] |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                                                  |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                                                   |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                                           |
{{</td>}}

**Source code**: [`apps/cacheStatus`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/cacheStatus)

## chifra serve

`chifra serve` delivers a JSON API for each of the `chifra` commands along with each of its options. It does this through `flame` server which is written in Go.

[Get help with the API here](https://www.tokenomics.io/api.html
).

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options.

### usage

`Usage:`    chifra serve  
`Purpose:`  Present each chifra command along with all of its options as a JSON api.

`Where:`

|          | Option                | Description                                      |
| -------- | --------------------- | ------------------------------------------------ |
|          | &#8208;&#8208;port    | specify the server's port (default ":8080")      |
| &#8208;v | &#8208;&#8208;verbose | set verbose level (optional level defaults to 1) |
| &#8208;h | &#8208;&#8208;help    | display this help screen                         |

`Notes:`

- To use the API, first open a new terminal window and run `chifra serve`
- Instead of the command line `chifra status index --details`, run `curl "http://localhost:8080/status?modes=index&details"`.
- Treat other commands similarly.

**Source code**: [`go-apps/flame`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/go-apps/flame)
## chifra scrape

The `chifra scrape` application creates TrueBlocks' index of address appearances -- the fundemental data structure of the entire system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. It is possible to start and stop `chifra scrape` as needed, but doing so means the scraper needs to catch up to the front of the chain, a process that may take some time depending on how frequently the scraper is run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

The scraper can scrape either the index only, previously created monitors only, both, or neither. If you specify `none`, timestamps will be scraped but nothing else. If you're scraping monitors, you may tell the system to cache traces and transactions. This will speed up access, but take addition hard drive space. You may also adjust the speed of operation on different machines with the `--sleep` and `--n_blocks` options. Finally, you may choose to optionally `--pin` each new chunk to IPFS.

### usage

`Usage:`    chifra scrape [-p|-s|-v|-h]  
`Purpose:`  Scan the chain and update the TrueBlocks index of appearances.

`Where:`

{{<td>}}
|          | Option                             | Description                                                                         |
| -------- | ---------------------------------- | ----------------------------------------------------------------------------------- |
| &#8208;p | &#8208;&#8208;pin                  | pin new chunks (and blooms) to IPFS (requires Pinata<br/>key and running IPFS node) |
| &#8208;s | &#8208;&#8208;sleep &lt;double&gt; | the number of seconds to sleep between passes (default<br/>14)                      |
| &#8208;v | &#8208;&#8208;verbose              | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help                 | display this help screen                                                            |
{{</td>}}

`Configurable Items:`

`n_blocks`: maximum number of blocks to process (defaults to 5000).
`n_block_procs`: number of concurrent block channels for blaze.
`n_addr_procs`: number of concurrent address channels for blaze.

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed (plus one) and decends as deeply as it can into the block's data. (This is why we need a `--tracing` node.) As address appearances are encountered, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records. As part of the consolidation, the scraper creates a Bloom filter representing the chunk. The Bloom filters are an order of magnitude or more smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: we all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software, for that matter) subsequently downloads the Bloom filters, queries them to determine which **chunks** need to be downloaded to the user's machine and thereby build a historical list of transacitons for a given address. This is accomplished while imposing a minimum amount of data on the end user's machine.

In future versions of the software, we will pin these shared chunks and blooms on end user's machines. They need the data for the software to operate and sharing it makes all user's better off. A naturally-born network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software works without an `--archive` node, but it works significantly better with one. An additional requirement for the software to work properly is an RPC that provides OpenEthereum's `trace_` routines. We've tested with OpenEthereum and Erigon.

Please see [this article](.) for more information about running the scraper and building and sharing the index of appearances.


**Source code**: [`apps/blockScrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/blockScrape)

## chifra init

When invoked, `chifra init` looks at a smart contract called **The Unchained Index** ([0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd](https://etherscan.io/address/0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd)). From this smart contract, it extracts a data item called `manifestHash`. The `manifestHash` is an IPFS hash that points to a file (a manifest) that contains every previously pinned Bloom filter and index chunk. TrueBlocks periodically publishes the manifest's hash to the smart contract. This makes the entire index both available for our software to use and impossible for us to withhold. Both of these aspects of the manifest are included by design.

If you stop `chifra init` before it finishes, it will pick up against where it left off the next time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will start where `init` finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

### usage

`Usage:`    chifra init  
`Purpose:`  Leech the Bloom filters from IPFS by first downloading the pin manifest from a smart contract and then downloading the blooms. Optionally `--pin` the resulting download in order to share it with others.

`Where:`

|          | Option                    | Description                                                               |
| -------- | ------------------------- | ------------------------------------------------------------------------- |
| &#8208;i | &#8208;&#8208;init        | initialize local index by downloading Bloom filters from pinning service  |
| &#8208;k | &#8208;&#8208;init_all    | initialize local index by downloading both Bloom filters and index chunks |
| &#8208;p | &#8208;&#8208;pin_locally | pin all local files in the index to an IPFS store (requires IPFS)         |
| &#8208;v | &#8208;&#8208;verbose     | set verbose level (optional level defaults to 1)                          |
| &#8208;h | &#8208;&#8208;help        | display this help screen                                                  |

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.
**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)
## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra pins [-l|-i|-n|-p|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                       |
| -------- | ----------------------------- | --------------------------------------------------------------------------------- |
| &#8208;l | &#8208;&#8208;list            | list the index and Bloom filter hashes from local manifest<br/>or pinning service |
| &#8208;i | &#8208;&#8208;init            | initialize local index by downloading Bloom filters<br/>from pinning service      |
| &#8208;n | &#8208;&#8208;init_all        | initialize local index by downloading both Bloom filters<br/>and index chunks     |
| &#8208;p | &#8208;&#8208;pin_locally     | pin all local files in the index to an IPFS store (requires<br/>IPFS)             |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                  |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                          |
{{</td>}}

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

