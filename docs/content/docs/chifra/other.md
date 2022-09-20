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
* `chifra explore` a quick way to open a blockchain explorer,
* `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of centralization and data quality, see explanation in its section).

{{< alert icon="👉" title="note about keys"
text="Note: some of these tools, like `ethslurp`, require an EtherScan key." >}}
[Follow these instructions to add a key to your config](/docs/install/install-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys)

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

