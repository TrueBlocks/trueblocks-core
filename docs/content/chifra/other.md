---
title: "Other"
description: ""
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
aliases:
 - "/docs/chifra/other"
menu:
  chifra:
    parent: commands
weight: 1800
toc: true
---
<!-- markdownlint-disable MD033 MD036 MD041 -->
The commands in the Other group provide useful miscellaneous features.

- `chifra explore` provides a quick way to open the configured blockchain explorer,
- `ethslurp` (an older tool) allows you call extract data from Etherscan.

To the right is a list of commands in this group. Click on a command to see its full documentation.

Note: Some of these tools, such as `ethslurp`, require an API key. [Follow these instructions](/docs/install/install-core/#3-update-the-configs-for-your-rpc-and-api-keys) to add a key to your config.
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

Data models produced by this tool:

- none

Links:

- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/explore)

## chifra slurp

<!-- markdownlint-disable MD041 -->
`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
[http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
for a given account. This is actually a shortcoming with Etherscan. It's too complicated to explain
here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull
any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
referenced website for more information.

```[plaintext]
Purpose:
  Fetch data from Etherscan for any address.

Usage:
  chifra slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp

Flags:
  -t, --types strings   which types of transactions to request
                        One or more of [ ext | int | token | nfts | 1155 | miner | uncles | all ]
  -p, --appearances     show only the blocknumber.tx_id appearances of the exported transactions
  -P, --per_page uint   the number of records to request on each page (default 5000)
  -s, --sleep float     seconds to sleep between requests (default 0.25)
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Portions of this software are Powered by Etherscan.io APIs.
```

Data models produced by this tool:

- [transaction](/data-model/chaindata/#transaction)

Links:

- [api docs](/api/#operation/other-slurp)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/slurp)

