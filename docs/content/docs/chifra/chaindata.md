---
title: "Chain data"
description: ""
lead: ""
date: 2021-10-05T22:20:10
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
weight: 1300
toc: true
---
The TrueBlocks tools extract raw blockchain data directly from the node.
You may extract block data, transactional data, receipts, logs, and traces.
Each tool has it own set of options, allowing you to get exactly the data that
you want.
## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts). You may optionally retrieve only the transaction hashes in the block (which is significantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behavior and chain-wide adoption analysis.

```[plaintext]
Purpose:
  Retrieve one or more blocks from the chain or local cache.

Usage:
  chifra blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)

Flags:
  -e, --hashes    display only transaction hashes, default is to display full transaction detail
  -U, --uncles    display uncle blocks (if any) instead of the requested block
  -t, --trace     export the traces from the block as opposed to the block data
  -a, --apps      display only the list of address appearances in the block
  -u, --uniq      display only the list of uniq address appearances in the block
  -n, --uniq_tx   display only the list of uniq address appearances in each transaction
  -c, --count     display the number of the lists of appearances for --apps, --uniq, or --uniq_tx
  -o, --cache     force a write of the block to the cache

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - blocks may be specified as either numbers or hashes.
  - special blocks are detailed under chifra when --list.
```

**Source code**: [`tools/getBlocks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getBlocks)

## chifra receipts

`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using `chifra transactions` and more focused data using `chifra logs`. It is included here for completeness, as the `receipt` is a fundamental data structure in Ethereum.

```[plaintext]
Purpose:
  Retrieve receipts for the given transaction(s).

Usage:
  chifra receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate   articulate the retrieved data if ABIs can be found

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash, a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
```

**Source code**: [`tools/getReceipts`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getReceipts)

## chifra logs

`chifra logs` returns the given transaction's logs. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data. The `--topic` and `--source` options allow you to filter your results.

```[plaintext]
Purpose:
  Retrieve logs for the given transaction(s).

Usage:
  chifra logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -t, --topic strings    filter by one or more log topics (not implemented)
  -s, --source strings   export only if the given address emitted the event (not implemented)
  -a, --articulate       articulate the retrieved data if ABIs can be found

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash, a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is not found, it will be used as a topic.
```

**Source code**: [`tools/getLogs`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getLogs)

## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated string of the same values used by `trace_fitler`.

```[plaintext]
Purpose:
  Retrieve traces for the given transaction(s).

Usage:
  chifra traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate      articulate the retrieved data if ABIs can be found
  -f, --filter string   call the node's trace_filter routine with bang-separated filter
  -d, --statediff       export state diff traces (not implemented)
  -c, --count           show the number of traces for the transaction only (fast)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash, a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
  - A state diff trace describes, for each modified address, what changed during that trace.
```

**Source code**: [`tools/getTraces`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTraces)

## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed. In the later case, supply a date (and optionally a time) and the block number that occurred at or just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexadecimal number or block hashes. You may specify any number of dates and/or blocks per invocation.

```[plaintext]
Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.

Usage:
  chifra when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes) (required)

Flags:
  -l, --list         export a list of the 'special' blocks
  -t, --timestamps   ignore other options and generate timestamps only

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].
```

**Source code**: [`tools/whenBlock`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/whenBlock)

