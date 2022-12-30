---
title: "Other"
description: ""
lead: ""
date: 2022-12-30T16:18:20
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
<!-- markdownlint-disable MD033 MD036 MD041 -->
These commands call some useful miscellaneous tools:

- `chifra explore` a quick way to open a blockchain explorer,
- `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of
centralization and data quality, see explanation in its section).

Note: some of these tools, like `ethslurp`, require an EtherScan key. [Follow these instructions
to add a key to your config](/docs/install/install-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys).

## chifra explore

<!-- markdownlint-disable MD041 -->
`chifra explore` opens Etherscan (and other explorers -- including our own) to the block identifier,
transaction identifier, or address you specify. It's a handy (configurable) way to open an explorer
from the command line, nothing more.

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

## chifra slurp

<!-- markdownlint-disable MD041 -->
`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
[http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
for a given account. This is actually a shortcoming with EtherScan. It's too complicated to explain
here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull
any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
referenced website for more information.

```[plaintext]
Purpose:
  Fetch data from EtherScan for any address.

Usage:
  chifra slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp

Flags:
  -t, --types strings   which types of transactions to request
                        One or more of [ ext | int | token | nfts | miner | uncles | all ]
  -p, --appearances     show only the blocknumber.tx_id appearances of the exported transactions
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - Portions of this software are Powered by Etherscan.io APIs.
```

**Source code**: [`internal/slurp`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/slurp)

