---
title: "Chain data"
description: "Access and cache blockchain-related data"
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
aliases:
 - "/docs/chifra/chaindata"
menu:
  chifra:
    parent: commands
weight: 21000
toc: true
---

The Chain Data group of tools extract raw blockchain data directly from the node. You may extract
block data, transactional data, receipts, logs, traces, and other information. Each tool has it own
set of options, allowing you to get exactly the data you need.

To the right is a list of commands in this group. Click on a command to see its full documentation.

## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the
TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts).
You may optionally retrieve only the transaction hashes in the block (which is significantly faster).
Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block.
TrueBlocks uses a similar feature internally to build its index of appearances. This type of data
is very insightful when studying end user behavior and chain-wide adoption analysis.

```[plaintext]
Purpose:
  Retrieve one or more blocks from the chain or local cache.

Usage:
  chifra blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)

Flags:
  -e, --hashes            display only transaction hashes, default is to display full transaction detail
  -c, --uncles            display uncle blocks (if any) instead of the requested block
  -t, --traces            export the traces from the block as opposed to the block data
  -u, --uniq              display a list of uniq address appearances per transaction
  -f, --flow string       for the --uniq option only, export only from or to (including trace from or to)
                          One of [ from | to | reward ]
  -l, --logs              display only the logs found in the block(s)
  -m, --emitter strings   for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings     for the --logs option only, filter logs to show only those with this topic(s)
  -i, --withdrawals       export the withdrawals from the block as opposed to the block data
  -a, --articulate        for the --logs option only, articulate the retrieved data if ABIs can be found
  -r, --big_range uint    for the --logs option only, allow for block ranges larger than 500 (default 500)
  -U, --count             display only the count of appearances for --addrs or --uniq
  -H, --ether             specify value in ether
  -w, --raw               report JSON data from the source with minimal processing
  -o, --cache             force the results of the query into the cache
  -D, --decache           removes related items from the cache
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - Blocks may be specified as either numbers or hashes.
  - Special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - For the --logs option, large block ranges may crash the node, use --big_range to specify a larger range.
  - The --decache option removes the block(s), all transactions in those block(s), and all traces in those transactions from the cache.
  - The --withdrawals option is only available on certain chains. It is ignored otherwise.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [block](/data-model/chaindata/#block)
- [blockcount](/data-model/chaindata/#blockcount)
- [log](/data-model/chaindata/#log)
- [logfilter](/data-model/chaindata/#logfilter)
- [message](/data-model/other/#message)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [withdrawal](/data-model/chaindata/#withdrawal)

Links:

- [api docs](/api/#operation/chaindata-blocks)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/blocks)

## chifra transactions

The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw`
option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers
per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt
and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those
encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attaches an array transaction traces to the output (if the node you're querying
has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances
instead of the underlying data (including uniq addresses in traces if enabled).

```[plaintext]
Purpose:
  Retrieve one or more transactions from the chain or local cache.

Usage:
  chifra transactions [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate        articulate the retrieved data if ABIs can be found
  -t, --traces            include the transaction's traces in the results
  -u, --uniq              display a list of uniq addresses found in the transaction
  -f, --flow string       for the uniq option only, export only from or to (including trace from or to)
                          One of [ from | to ]
  -l, --logs              display only the logs found in the transaction(s)
  -m, --emitter strings   for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings     for the --logs option only, filter logs to show only those with this topic(s)
  -H, --ether             specify value in ether
  -w, --raw               report JSON data from the source with minimal processing
  -o, --cache             force the results of the query into the cache
  -D, --decache           removes related items from the cache
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - The --decache option removes the all transaction(s) and all traces in those transactions from the cache.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [function](/data-model/other/#function)
- [log](/data-model/chaindata/#log)
- [message](/data-model/other/#message)
- [parameter](/data-model/other/#parameter)
- [transaction](/data-model/chaindata/#transaction)

Links:

- [api docs](/api/#operation/chaindata-transactions)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/transactions)

## chifra receipts

`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers
per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those
encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using
`chifra transactions` and more focused data using `chifra logs`. It is included here for
completeness, as the `receipt` is a fundamental data structure in Ethereum.

```[plaintext]
Purpose:
  Retrieve receipts for the given transaction(s).

Usage:
  chifra receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate   articulate the retrieved data if ABIs can be found
  -w, --raw          report JSON data from the source with minimal processing
  -o, --cache        force the results of the query into the cache
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
```

Data models produced by this tool:

- [function](/data-model/other/#function)
- [parameter](/data-model/other/#parameter)
- [receipt](/data-model/chaindata/#receipt)

Links:

- [api docs](/api/#operation/chaindata-receipts)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/receipts)

## chifra logs

`chifra logs` returns the given transaction's logs. You may specify multiple transaction identifiers
per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe
the reported data. The `--topic` and `--source` options allow you to filter your results.

```[plaintext]
Purpose:
  Retrieve logs for the given transaction(s).

Usage:
  chifra logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -m, --emitter strings   filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings     filter logs to show only those with this topic(s)
  -a, --articulate        articulate the retrieved data if ABIs can be found
  -w, --raw               report JSON data from the source with minimal processing
  -o, --cache             force the results of the query into the cache
  -D, --decache           removes related items from the cache
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if it is not, the hash will be used as a topic.
```

Data models produced by this tool:

- [function](/data-model/other/#function)
- [log](/data-model/chaindata/#log)
- [logfilter](/data-model/chaindata/#logfilter)
- [message](/data-model/other/#message)
- [parameter](/data-model/other/#parameter)

Links:

- [api docs](/api/#operation/chaindata-logs)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/logs)

## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction
identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe
the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated
string of the same values used by `trace_fitler`.

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
  -U, --count           display only the number of traces for the transaction (fast)
  -H, --ether           specify value in ether
  -w, --raw             report JSON data from the source with minimal processing
  -o, --cache           force the results of the query into the cache
  -D, --decache         removes related items from the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose output
  -h, --help            display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
```

Data models produced by this tool:

- [function](/data-model/other/#function)
- [message](/data-model/other/#message)
- [parameter](/data-model/other/#parameter)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [tracecount](/data-model/chaindata/#tracecount)
- [tracefilter](/data-model/chaindata/#tracefilter)
- [traceresult](/data-model/chaindata/#traceresult)

Links:

- [api docs](/api/#operation/chaindata-traces)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/traces)

## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block
occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed.
In the later case, supply a date (and optionally a time) and the block number that occurred at or
just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are
optional, and if omitted, default to zero in each case. Block numbers may be specified as either
integers or hexadecimal number or block hashes. You may specify any number of dates and/or blocks
per invocation.

```[plaintext]
Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.

Usage:
  chifra when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes)

Flags:
  -l, --list         export a list of the 'special' blocks
  -t, --timestamps   display or process timestamps
  -U, --count        with --timestamps only, returns the number of timestamps in the cache
  -r, --repair       with --timestamps only, repairs block(s) in the block range by re-querying from the chain
  -c, --check        with --timestamps only, checks the validity of the timestamp data
  -u, --update       with --timestamps only, bring the timestamp database forward to the latest block
  -d, --deep         with --timestamps --check only, verifies timestamps from on chain (slow)
  -o, --cache        force the results of the query into the cache
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Block numbers, timestamps, or dates in the future are estimated with 13 second blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].
```

Data models produced by this tool:

- [message](/data-model/other/#message)
- [namedblock](/data-model/chaindata/#namedblock)
- [timestamp](/data-model/chaindata/#timestamp)
- [timestampcount](/data-model/chaindata/#timestampcount)

Links:

- [api docs](/api/#operation/chaindata-when)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/when)

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
