---
title: "Chain data"
description: ""
lead: ""
date: 2023-01-03T21:42:38
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  data:
    parent: "collections"
weight: 1200
toc: true
---

<!-- markdownlint-disable MD033 MD036 MD041 -->
The following data structures describe the output of various TrueBlocks blockchain queries. These
data structures basically mimic the data available directly from the RPC.

Each data structure is created by one or more tools which are detailed below.

## Block

<!-- markdownlint-disable MD033 MD036 MD041 -->
`chifra blocks` returns top level data specified block. You can also include an array for the
blocks' transactions.

The following commands produce and manage Blocks:

- [chifra blocks](/docs/chifra/chaindata/#chifra-blocks)

Block data is made of the following fields:

| Field         | Description                                                   | Type                                                |
| ------------- | ------------------------------------------------------------- | --------------------------------------------------- |
| gasLimit      | the system-wide maximum amount of gas permitted in this block | gas                                                 |
| hash          | the hash of the current block                                 | hash                                                |
| blockNumber   | the number of the block                                       | blknum                                              |
| parentHash    | hash of previous block                                        | hash                                                |
| miner         | Address of block's winning miner                              | address                                             |
| difficulty    | the computational difficulty at this block                    | uint64                                              |
| timestamp     | the Unix timestamp of the object                              | timestamp                                           |
| transactions  | a possibly empty array of transactions or transaction hashes  | [Transaction[]](/data-model/chaindata/#transaction) |
| baseFeePerGas | the base fee for this block                                   | wei                                                 |
| finalized     | flag indicating the system considers this data final          | bool                                                |
| unclesCnt     | the number of uncles in this block                            | uint64                                              |

## Transaction

<!-- markdownlint-disable MD033 MD036 MD041 -->
Transactions represent eth transfers to and from other addresses.

Most of the fields that TrueBlocks returns are standard to all eth transaction. However, one field
is very interesting: `articulatedTx` provides a human readable output of the `input` field.

This is a very powerful way to understand the story behind a smart contract.

The following commands produce and manage Transactions:

- [chifra transactions](/docs/chifra/chaindata/#chifra-transactions)
- [chifra slurp](/docs/chifra/other/#chifra-slurp)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Transaction data is made of the following fields:

| Field            | Description                                                                                           | Type                                                     |
| ---------------- | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------- |
| hash             | The hash of the transaction                                                                           | hash                                                     |
| blockHash        | The hash of the block containing this transaction                                                     | hash                                                     |
| blockNumber      | the number of the block                                                                               | blknum                                                   |
| transactionIndex | the zero-indexed position of the transaction in the block                                             | blknum                                                   |
| nonce            | sequence number of the transactions sent by the sender                                                | uint64                                                   |
| timestamp        | the Unix timestamp of the object                                                                      | timestamp                                                |
| from             | address from which the transaction was sent                                                           | address                                                  |
| to               | address to which the transaction was sent                                                             | address                                                  |
| value            | the amount of wei sent with this transactions                                                         | wei                                                      |
| gas              | the maximum number of gas allowed for this transaction                                                | gas                                                      |
| gasPrice         | the number of wei per unit of gas the sender is willing to spend                                      | gas                                                      |
| input            | byte data either containing a message or funcational data for a smart contracts. See the --articulate | bytes                                                    |
| receipt          |                                                                                                       | [Receipt](/data-model/chaindata/#receipt)                |
| statements       | array of reconciliations                                                                              | [Reconciliation[]](/data-model/accounts/#reconciliation) |
| articulatedTx    |                                                                                                       | [Function](/data-model/other/#function)                  |
| compressedTx     | truncated, more readable version of the articulation                                                  | string                                                   |
| hasToken         | `true` if the transaction is token related, `false` otherwise                                         | uint8                                                    |
| finalized        | flag indicating the system considers this data final                                                  | bool                                                     |
| extraData        |                                                                                                       | string                                                   |
| isError          | `true` if the transaction ended in error, `false` otherwise                                           | uint8                                                    |
| date             |                                                                                                       | datetime                                                 |

## Transfer

<!-- markdownlint-disable MD033 MD036 MD041 -->
The `transfer` data model is produced by the reconciliation process of `chifra export`. It
represents, for all intents and purposes, the asset value transfer produced by nearly all value
transfers on any blockchain. While the details of various token transfers are varied, the
underlying mechanism is through token transfers. In our accounting, we focus on the token transfers
and label each such transfer on the `input` data or `event topic` data. In this way, our accounting
remains relatively easy (we only reconcile tokens and ETH), but we cover every conceivable token
asset transfer of any type.

The following commands produce and manage Transfers:

- [chifra transactions](/docs/chifra/chaindata/#chifra-transactions)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Transfer data is made of the following fields:

| Field            | Description                                                                                    | Type      |
| ---------------- | ---------------------------------------------------------------------------------------------- | --------- |
| blockNumber      | the number of the block                                                                        | blknum    |
| transactionIndex | the zero-indexed position of the transaction in the block                                      | blknum    |
| logIndex         | the zero-indexed position of the log in the transaction                                        | blknum    |
| transactionHash  | the hash of the transaction that triggered this reconciliation                                 | hash      |
| timestamp        | the Unix timestamp of the object                                                               | timestamp |
| date             | a calculated field -- the date of this transaction                                             | datetime  |
| sender           | the initiator of the transfer (the sender)                                                     | address   |
| recipient        | the receiver of the transfer (the recipient)                                                   | address   |
| assetAddr        | 0xeeee...eeee for ETH reconcilations, the token address otherwise                              | address   |
| assetSymbol      | either ETH, WEI or the symbol of the asset being reconciled as queried from the chain          | string    |
| decimals         | Equivalent to the queried value of `decimals` from an ERC20 contract or, if ETH or WEI then 18 | uint64    |
| amount           | the amount of the transfer in the units of the asset                                           | uint256   |
| spotPrice        | The on-chain price in USD (or if a token in ETH, or zero) at the time of the transaction       | double    |
| priceSource      | The on-chain source from which the spot price was taken                                        | string    |
| encoding         | The four-byte encoding of the transaction's function call                                      | string    |

## Receipt

<!-- markdownlint-disable MD033 MD036 MD041 -->
Receipts record the amount of gas used for a transaction among other things. If the transaction
succeeded, a receipt might also have logs.

If the `to` address of a transaction is `0x0`, the `input` data is considered to be the source
code (byte code) of a smart contract. In this case, if the creation of the contract succeeds,
the `contractAddress` field of the receipt carries the address of the newly created contract.

The following commands produce and manage Receipts:

- [chifra receipts](/docs/chifra/chaindata/#chifra-receipts)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Receipt data is made of the following fields:

| Field           | Description                                                                | Type                                |
| --------------- | -------------------------------------------------------------------------- | ----------------------------------- |
| status          | `1` on transaction suceess, `null` if tx preceeds Byzantium, `0` otherwise | uint32                              |
| contractAddress | the address of the newly created contract, if any                          | address                             |
| gasUsed         | the amount of gas actually used by the transaction                         | gas                                 |
| logs            | a possibly empty array of logs                                             | [Log[]](/data-model/chaindata/#log) |

## Log

<!-- markdownlint-disable MD033 MD036 MD041 -->
Logs appear in a possibly empty array in the transaction's receipt. They are only created if the
underlying transaction suceeded. In the case where the transaction failed, no logs will appear in
the receipt. Logs are only ever generated during transactions whose `to` address is a smart
contract.

The following commands produce and manage Logs:

- [chifra logs](/docs/chifra/chaindata/#chifra-logs)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Log data is made of the following fields:

| Field            | Description                                                                                       | Type                                    |
| ---------------- | ------------------------------------------------------------------------------------------------- | --------------------------------------- |
| blockNumber      | the number of the block                                                                           | blknum                                  |
| transactionIndex | the zero-indexed position of the transaction in the block                                         | blknum                                  |
| logIndex         | the zero-indexed position of this log relative to the block                                       | blknum                                  |
| transactionHash  | the hash of the transction                                                                        | hash                                    |
| timestamp        | the timestamp of the block this log appears in                                                    | timestamp                               |
| address          | the smart contract that emitted this log                                                          | address                                 |
| topics           | The first topic hashes event signature of the log, up to 3 additional index parameters may appear | topic[]                                 |
| data             | any remaining un-indexed parameters to the event                                                  | bytes                                   |
| articulatedLog   | a human-readable version of the topic and data fields                                             | [Function](/data-model/other/#function) |
| compressedLog    | a truncated, more readable version of the articulation                                            | string                                  |

## Trace

<!-- markdownlint-disable MD033 MD036 MD041 -->
The deepest layer of the Ethereum data is the trace. Every transaction has at least one trace which
is itself a record of the transaction. If the `to` address of the transaction is a smart contract,
other traces may appear, if, for example, that smart contract calls other smart contracts.

Traces may be arbitrarily deep (up to the gasLimit) and ultimately represent a tree of function
calls. Some transactions have 100s of traces. The format of the trace is similar to the transaction
itself have a trace `action` (which contains `from`, `to`, `value` like the transaction) and the
trace `result` (containing `gasUsed` like the receipt).

The following commands produce and manage Traces:

- [chifra traces](/docs/chifra/chaindata/#chifra-traces)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Trace data is made of the following fields:

| Field            | Description                                               | Type                                              |
| ---------------- | --------------------------------------------------------- | ------------------------------------------------- |
| blockHash        | The hash of the block containing this trace               | hash                                              |
| blockNumber      | the number of the block                                   | blknum                                            |
| timestamp        | the timestamp of the block                                | timestamp                                         |
| transactionHash  | the transaction's hash containing this trace              | hash                                              |
| transactionIndex | the zero-indexed position of the transaction in the block | blknum                                            |
| traceAddress     | a particular trace's address in the trace tree            | string[]                                          |
| subtraces        | the number of children traces that the trace hash         | uint64                                            |
| type             | the type of the trace                                     | string                                            |
| action           | the trace action for this trace                           | [TraceAction](/data-model/chaindata/#traceaction) |
| result           | the trace result of this trace                            | [TraceResult](/data-model/chaindata/#traceresult) |
| articulatedTrace | human readable version of the trace action input data     | [Function](/data-model/other/#function)           |
| compressedTrace  | a compressed string version of the articulated trace      | string                                            |

## TraceAction

<!-- markdownlint-disable MD033 MD036 MD041 -->
Other than the first trace which is the trace of the transaction itself, traces represent calls
into smart contracts. Because of this, `trace actions` closely resemble the fields of the
[transaction](#transactions).

The following commands produce and manage TraceActions:

- [chifra traces](/docs/chifra/chaindata/#chifra-traces)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Traceaction data is made of the following fields:

| Field         | Description                                                                | Type    |
| ------------- | -------------------------------------------------------------------------- | ------- |
| from          | address from which the trace was sent                                      | address |
| to            | address to which the trace was sent                                        | address |
| gas           | the maximum number of gas allowed for this trace                           | gas     |
| input         | an encoded version of the function call                                    | bytes   |
| callType      | the type of call                                                           | string  |
| refundAddress | if the call type is self-destruct, the address to which the refund is sent | address |

## TraceResult

<!-- markdownlint-disable MD033 MD036 MD041 -->
As mentioned above, other than the first trace, traces represent calls into other smart contracts.
Because of this, the trace results closely resembles the fields of the [receipt](#receipts).

The following commands produce and manage TraceResults:

- [chifra traces](/docs/chifra/chaindata/#chifra-traces)
- [chifra export](/docs/chifra/accounts/#chifra-export)

Traceresult data is made of the following fields:

| Field       | Description                                                                    | Type    |
| ----------- | ------------------------------------------------------------------------------ | ------- |
| newContract | Address of new contract, if any                                                | address |
| code        | if this trace is creating a new smart contract, the byte code of that contract | bytes   |
| gasUsed     | the amount of gas used by this trace                                           | gas     |
| output      | the result of the call of this trace                                           | bytes   |

## DatedBlock

<!-- markdownlint-disable MD033 MD036 MD041 -->
Left to its own devices, the blockchain would try to convince us that only hashes and bytes are
important, but being human beings we know that this is not true. TrueBlocks `articulates` various
types of data with [chifra names](/docs/chifra/accounts/#chifra-names) detailing the names for
addresses, `-articulate` describing the Functions and Events of a transaction, and
[chifra when](/docs/chifra/chaindata/#chifra-when) describing dated blocks. Dated blocks assign a
human-readable date to blocks given block numbers or timestamps and visa versa.

The following commands produce and manage DatedBlocks:

- [chifra when](/docs/chifra/chaindata/#chifra-when)

Datedblock data is made of the following fields:

| Field       | Description                         | Type      |
| ----------- | ----------------------------------- | --------- |
| blockNumber | the number of the block             | blknum    |
| timestamp   | the Unix timestamp of the block     | timestamp |
| date        | Human readable version of timestamp | datetime  |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                         | Notes          |
| --------- | ----------------------------------- | -------------- |
| address   | an '0x'-prefixed 20-byte hex string | lowercase      |
| blknum    | an alias for a uint64               |                |
| bool      | either `true`, `false`, `1`, or `0` |                |
| bytes     | an arbitrarily long string of bytes |                |
| datetime  | a JSON formatted date               | as a string    |
| double    | a double precision float            | 64 bits        |
| gas       | an unsigned big number              | as a string    |
| hash      | an '0x'-prefixed 32-byte hex string | lowercase      |
| string    | a normal character string           |                |
| timestamp | a 64-bit unsigned integer           | Unix timestamp |
| uint256   | a 256-bit unsigned integer          |                |
| uint32    | a 32-bit unsigned integer           |                |
| uint64    | a 64-bit unsigned integer           |                |
| uint8     | an alias for the boolean type       |                |
| wei       | an unsigned big number              | as a string    |
