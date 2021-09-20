---
title: "Chain Data"
description: ""
lead: ""
date: 2021-09-19T21:39:18
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
The chaindata tools extract raw blockchain data directly from the node.
You may extract block data, transactional data, receipts, logs, and traces.
Each tool has it own set of options, allowing you to get exactly the data that
you want.
## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts). You may optionally retreive only the transaction hashes in the block (which is signifcantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behaviour and chain-wide adoption analysis.

### usage

`Usage:`    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Retrieve one or more blocks from the chain or local cache.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                         |
| -------- | ----------------------------- | ----------------------------------------------------------------------------------- |
|          | blocks                        | a space-separated list of one or more block identifiers<br/>(required)              |
| &#8208;e | &#8208;&#8208;hashes          | display only transaction hashes, default is to display<br/>full transaction detail  |
| &#8208;U | &#8208;&#8208;uncles          | display uncle blocks (if any) instead of the requested<br/>block                    |
| &#8208;t | &#8208;&#8208;trace           | export the traces from the block as opposed to the<br/>block data                   |
| &#8208;a | &#8208;&#8208;apps            | display only the list of address appearances in the<br/>block                       |
| &#8208;u | &#8208;&#8208;uniq            | display only the list of uniq address appearances in<br/>the block                  |
| &#8208;n | &#8208;&#8208;uniq_tx         | display only the list of uniq address appearances in<br/>each transaction           |
| &#8208;c | &#8208;&#8208;count           | display the number of the lists of appearances for<br/>--apps, --uniq, or --uniq_tx |
| &#8208;o | &#8208;&#8208;cache           | force a write of the block to the cache                                             |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                   |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                            |
{{</td>}}

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

**Source code**: [`tools/getBlocks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getBlocks)

## chifra transactions

The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw` option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attachs an array transaction traces to the output (if the node you're querying has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances instead of the underlying data (including uniq addresses in traces if enabled).

### usage

`Usage:`    chifra transactions [-a|-t|-u|-r|-o|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve one or more transactions from the chain or local cache.

`Where:`

{{<td>}}
|          | Option                                  | Description                                                                                  |
| -------- | --------------------------------------- | -------------------------------------------------------------------------------------------- |
|          | transactions                            | a space-separated list of one or more transaction identifiers<br/>(required)                 |
| &#8208;a | &#8208;&#8208;articulate                | articulate the retrieved data if ABIs can be found                                           |
| &#8208;t | &#8208;&#8208;trace                     | include the transaction's traces in the results                                              |
| &#8208;u | &#8208;&#8208;uniq                      | display a list of uniq addresses found in the transaction<br/>instead of the underlying data |
| &#8208;r | &#8208;&#8208;reconcile &lt;address&gt; | reconcile the transaction as per the provided address                                        |
| &#8208;o | &#8208;&#8208;cache                     | force the results of the query into the tx cache (and<br/>the trace cache if applicable)     |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;           | export format, one of [none, json, txt, csv, api]                                            |
| &#8208;v | &#8208;&#8208;verbose                   | set verbose level (optional level defaults to 1)                                             |
| &#8208;h | &#8208;&#8208;help                      | display this help screen                                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

**Source code**: [`tools/getTrans`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTrans)

## chifra receipts

`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using `chifra transactions` and more focused data using `chifra logs`. It is included here for completeness, as the `receipt` is a fundamental data structure in Ethereum.

### usage

`Usage:`    chifra receipts [-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve receipts for the given transaction(s).

`Where:`

{{<td>}}
|          | Option                        | Description                                                                  |
| -------- | ----------------------------- | ---------------------------------------------------------------------------- |
|          | transactions                  | a space-separated list of one or more transaction identifiers<br/>(required) |
| &#8208;a | &#8208;&#8208;articulate      | articulate the retrieved data if ABIs can be found                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                            |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                             |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

**Source code**: [`tools/getReceipts`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getReceipts)

## chifra logs

`chifra logs` returns the given transaction's logs. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data. The `--topic` and `--source` options allow you to filter your results.

### usage

`Usage:`    chifra logs [-t|-s|-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve logs for the given transaction(s).

`Where:`

{{<td>}}
|          | Option                            | Description                                                                  |
| -------- | --------------------------------- | ---------------------------------------------------------------------------- |
|          | transactions                      | a space-separated list of one or more transaction identifiers<br/>(required) |
| &#8208;t | &#8208;&#8208;topic &lt;hash&gt;  | filter by one or more log topics (not implemented)                           |
| &#8208;s | &#8208;&#8208;source &lt;addr&gt; | export only if the given address emitted the event<br/>(not implemented)     |
| &#8208;a | &#8208;&#8208;articulate          | articulate the retrieved data if ABIs can be found                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;     | export format, one of [none, json, txt, csv, api]                            |
| &#8208;v | &#8208;&#8208;verbose             | set verbose level (optional level defaults to 1)                             |
| &#8208;h | &#8208;&#8208;help                | display this help screen                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.
- If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is
  not found, it will be used as a topic.

**Source code**: [`tools/getLogs`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getLogs)

## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated string of the same values used by `trace_fitler`.

### usage

`Usage:`    chifra traces [-a|-f|-d|-c|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve traces for the given transaction(s).

`Where:`

{{<td>}}
|          | Option                           | Description                                                                  |
| -------- | -------------------------------- | ---------------------------------------------------------------------------- |
|          | transactions                     | a space-separated list of one or more transaction identifiers<br/>(required) |
| &#8208;a | &#8208;&#8208;articulate         | articulate the retrieved data if ABIs can be found                           |
| &#8208;f | &#8208;&#8208;filter &lt;str&gt; | call the node's `trace_filter` routine with bang-seperated<br/>filter        |
| &#8208;d | &#8208;&#8208;statediff          | export state diff traces (not implemented)                                   |
| &#8208;c | &#8208;&#8208;count              | show the number of traces for the transaction only<br/>(fast)                |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                            |
| &#8208;v | &#8208;&#8208;verbose            | set verbose level (optional level defaults to 1)                             |
| &#8208;h | &#8208;&#8208;help               | display this help screen                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.
- A bang seperated filter has the following fields (at least one of which is required) and is separated
  with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
- A state diff trace describes, for each modified address, what changed during that trace.

`Configurable Items:`

- `skip_ddos`: skip over the 2016 ddos during export ('on' by default).
- `max`: if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250).

**Source code**: [`tools/getTraces`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTraces)

## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed. In the later case, supply a date (and optionally a time) and the block number that occurred at or just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexidecimal number or block hashes. You may specify any number of dates and/or blocks per invocation.

### usage

`Usage:`    chifra when [-l|-t|-v|-h] &lt; block | date &gt; [ block... | date... ]  
`Purpose:`  Find block(s) based on date, blockNum, timestamp, or 'special'.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                       |
| -------- | ----------------------------- | --------------------------------------------------------------------------------- |
|          | block_list                    | one or more dates, block numbers, hashes, or special<br/>named blocks (see notes) |
| &#8208;l | &#8208;&#8208;list            | export a list of the 'special' blocks                                             |
| &#8208;t | &#8208;&#8208;timestamps      | ignore other options and generate timestamps only                                 |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                  |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                          |
{{</td>}}

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

**Source code**: [`tools/whenBlock`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/whenBlock)

