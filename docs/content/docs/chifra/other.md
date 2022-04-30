---
title: "Other"
description: ""
lead: ""
date: 2022-04-30T13:40:54
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
weight: 1900
toc: true
---
These commands call some useful miscellaneous tools:
* `chifra quotes`, a rudimentary pricing tool,
* `chifra explore` a quick way to open a blockchain explorer,
* `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of centralization and data quality, see explanation in its section).

{{< alert icon="👉" title="note about keys"
text="Note: some of these tools, like `ethslurp`, require an EtherScan key." >}}
[Follow these instructions to add a key to your config](/docs/install/install-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys)

## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

```[plaintext]
Purpose:
  Update or display Ethereum price data, this tool has been deprecated.

Usage:
  chifra quotes [flags]

Flags:
  -u, --update          freshen price database
  -p, --period string   increment of display
                        One of [ 5 | 15 | 30 | 60 | 120 | 240 | 1440 | 10080 | hourly | daily | weekly ]
  -a, --pair string     which price pair to freshen or list (see Poloniex)
  -e, --feed string     the feed for the price data
                        One of [ poloniex | maker | tellor ]
  -x, --fmt string      export format, one of [none|json*|txt|csv|api]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - Due to restrictions from Poloniex, this tool retrieves only 30 days of data at a time. You must repeatedly run this command until the data is up-to-date.
```

**Source code**: [`internal/quotes`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/quotes)

## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

```[plaintext]
Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.

Usage:
  chifra explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier

Flags:
  -l, --local    open the local TrueBlocks explorer
  -g, --google   search google excluding popular blockchain explorers
  -h, --help     display this help screen
```

**Source code**: [`internal/explore`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/explore)

