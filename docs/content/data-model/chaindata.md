---
title: "Chain data"
description: "Access and cache blockchain-related data"
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
menu:
  data:
    parent: collections
weight: 21000
toc: true
---

The following data structures describe the output of various TrueBlocks blockchain queries. These
data structures basically mimic the data available directly from the RPC.

Each data structure is created by one or more tools which are detailed below.

## Block

`chifra blocks` returns top level data specified block. You can also include an array for the
blocks' transactions.

The following commands produce and manage Blocks:

- [chifra blocks](/chifra/chaindata/#chifra-blocks)

Blocks consist of the following fields:

| Field         | Description                                                   | Type                                                |
| ------------- | ------------------------------------------------------------- | --------------------------------------------------- |
| gasLimit      | the system-wide maximum amount of gas permitted in this block | gas                                                 |
| hash          | the hash of the current block                                 | hash                                                |
| blockNumber   | the number of the block                                       | blknum                                              |
| parentHash    | hash of previous block                                        | hash                                                |
| miner         | address of block's winning miner                              | address                                             |
| difficulty    | the computational difficulty at this block                    | value                                               |
| timestamp     | the Unix timestamp of the object                              | timestamp                                           |
| date          | the timestamp as a date (calculated)                          | datetime                                            |
| transactions  | a possibly empty array of transactions                        | [Transaction[]](/data-model/chaindata/#transaction) |
| baseFeePerGas | the base fee for this block                                   | gas                                                 |
| uncles        | a possibly empty array of uncle hashes                        | hash[]                                              |
| withdrawals   | a possibly empty array of withdrawals (post Shanghai)         | [Withdrawal[]](/data-model/chaindata/#withdrawal)   |

## Transaction

Transactions represent eth transfers to and from other addresses.

Most of the fields that TrueBlocks returns are standard to all eth transaction. However, one field
is very interesting: `articulatedTx` provides a human readable output of the `input` field.

This is a very powerful way to understand the story behind a smart contract.

The following commands produce and manage Transactions:

- [chifra transactions](/chifra/chaindata/#chifra-transactions)
- [chifra export](/chifra/accounts/#chifra-export)

Transactions consist of the following fields:

| Field            | Description                                                                                           | Type                                           |
| ---------------- | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------- |
| hash             | the hash of the transaction                                                                           | hash                                           |
| blockHash        | the hash of the block containing this transaction                                                     | hash                                           |
| blockNumber      | the number of the block                                                                               | blknum                                         |
| transactionIndex | the zero-indexed position of the transaction in the block                                             | txnum                                          |
| nonce            | sequence number of the transactions sent by the sender                                                | value                                          |
| timestamp        | the Unix timestamp of the object                                                                      | timestamp                                      |
| date             | the timestamp as a date (calculated)                                                                  | datetime                                       |
| from             | address from which the transaction was sent                                                           | address                                        |
| to               | address to which the transaction was sent                                                             | address                                        |
| value            | the amount of wei sent with this transactions                                                         | wei                                            |
| ether            | if --ether is specified, the value in ether (calculated)                                              | ether                                          |
| gas              | the maximum number of gas allowed for this transaction                                                | gas                                            |
| gasPrice         | the number of wei per unit of gas the sender is willing to spend                                      | gas                                            |
| input            | byte data either containing a message or funcational data for a smart contracts. See the --articulate | bytes                                          |
| receipt          |                                                                                                       | [Receipt](/data-model/chaindata/#receipt)      |
| statements       | array of reconciliations (calculated)                                                                 | [Statement[]](/data-model/accounts/#statement) |
| articulatedTx    |                                                                                                       | [Function](/data-model/other/#function)        |
| hasToken         | `true` if the transaction is token related, `false` otherwise                                         | bool                                           |
| isError          | `true` if the transaction ended in error, `false` otherwise                                           | bool                                           |
| compressedTx     | truncated, more readable version of the articulation (calculated)                                     | string                                         |

## Withdrawal

`withdrawals` is an array present in post-Shanghai blocks representing Consensys layer staking reward withdrawals. Note that the amount present is in Gwei. The `withdrawals` array is not present in pre-Shanghai blocks.

The following commands produce and manage Withdrawals:

- [chifra blocks](/chifra/chaindata/#chifra-blocks)
- [chifra export](/chifra/accounts/#chifra-export)

Withdrawals consist of the following fields:

| Field          | Description                                                                                                          | Type      |
| -------------- | -------------------------------------------------------------------------------------------------------------------- | --------- |
| address        | the recipient for the withdrawn ether                                                                                | address   |
| amount         | a nonzero amount of ether given in gwei (1e9 wei)                                                                    | wei       |
| ether          | if --ether is specified, the amount in ether (calculated)                                                            | ether     |
| blockNumber    | the number of this block                                                                                             | blknum    |
| index          | a monotonically increasing zero-based index that increments by 1 per withdrawal to uniquely identify each withdrawal | value     |
| timestamp      | the timestamp for this block                                                                                         | timestamp |
| date           | the timestamp as a date (calculated)                                                                                 | datetime  |
| validatorIndex | the validator_index of the validator on the consensus layer the withdrawal corresponds to                            | value     |

## Receipt

Receipts record the amount of gas used for a transaction among other things. If the transaction
succeeded, a receipt might also have logs.

If the `to` address of a transaction is `0x0`, the `input` data is considered to be the source
code (byte code) of a smart contract. In this case, if the creation of the contract succeeds,
the `contractAddress` field of the receipt carries the address of the newly created contract.

The following commands produce and manage Receipts:

- [chifra receipts](/chifra/chaindata/#chifra-receipts)
- [chifra export](/chifra/accounts/#chifra-export)

Receipts consist of the following fields:

| Field            | Description                                                                | Type                                |
| ---------------- | -------------------------------------------------------------------------- | ----------------------------------- |
| blockHash        |                                                                            | hash                                |
| blockNumber      |                                                                            | blknum                              |
| contractAddress  | the address of the newly created contract, if any                          | address                             |
| gasUsed          | the amount of gas actually used by the transaction                         | gas                                 |
| isError          |                                                                            | bool                                |
| logs             | a possibly empty array of logs                                             | [Log[]](/data-model/chaindata/#log) |
| status           | `1` on transaction suceess, `null` if tx preceeds Byzantium, `0` otherwise | value                               |
| transactionHash  |                                                                            | hash                                |
| transactionIndex |                                                                            | txnum                               |

## Log

Logs appear in a possibly empty array in the transaction's receipt. They are only created if the
underlying transaction suceeded. In the case where the transaction failed, no logs will appear in
the receipt. Logs are only ever generated during transactions whose `to` address is a smart
contract.

The following commands produce and manage Logs:

- [chifra logs](/chifra/chaindata/#chifra-logs)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra blocks](/chifra/chaindata/#chifra-blocks)
- [chifra transactions](/chifra/chaindata/#chifra-transactions)

Logs consist of the following fields:

| Field            | Description                                                                                       | Type                                    |
| ---------------- | ------------------------------------------------------------------------------------------------- | --------------------------------------- |
| blockNumber      | the number of the block                                                                           | blknum                                  |
| transactionIndex | the zero-indexed position of the transaction in the block                                         | txnum                                   |
| logIndex         | the zero-indexed position of this log relative to the block                                       | lognum                                  |
| timestamp        | the timestamp of the block this log appears in                                                    | timestamp                               |
| date             | the timestamp as a date (calculated)                                                              | datetime                                |
| address          | the smart contract that emitted this log                                                          | address                                 |
| topics           | the first topic hashes event signature of the log, up to 3 additional index parameters may appear | topic[]                                 |
| data             | any remaining un-indexed parameters to the event                                                  | bytes                                   |
| transactionHash  | the hash of the transction                                                                        | hash                                    |
| blockHash        | the hash of the block                                                                             | hash                                    |
| articulatedLog   | a human-readable version of the topic and data fields                                             | [Function](/data-model/other/#function) |
| compressedLog    | a truncated, more readable version of the articulation (calculated)                               | string                                  |

## Trace

The deepest layer of the Ethereum data is the trace. Every transaction has at least one trace which
is itself a record of the transaction. If the `to` address of the transaction is a smart contract,
other traces may appear, if, for example, that smart contract calls other smart contracts.

Traces may be arbitrarily deep (up to the gasLimit) and ultimately represent a tree of function
calls. Some transactions have 100s of traces. The format of the trace is similar to the transaction
itself have a trace `action` (which contains `from`, `to`, `value` like the transaction) and the
trace `result` (containing `gasUsed` like the receipt).

The following commands produce and manage Traces:

- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra blocks](/chifra/chaindata/#chifra-blocks)

Traces consist of the following fields:

| Field            | Description                                                       | Type                                              |
| ---------------- | ----------------------------------------------------------------- | ------------------------------------------------- |
| blockHash        | the hash of the block containing this trace                       | hash                                              |
| blockNumber      | the number of the block                                           | blknum                                            |
| timestamp        | the timestamp of the block                                        | timestamp                                         |
| date             | the timestamp as a date (calculated)                              | datetime                                          |
| transactionHash  | the transaction's hash containing this trace                      | hash                                              |
| transactionIndex | the zero-indexed position of the transaction in the block         | txnum                                             |
| traceAddress     | a particular trace's address in the trace tree                    | uint64[]                                          |
| subtraces        | the number of children traces that the trace hash                 | uint64                                            |
| type             | the type of the trace                                             | string                                            |
| action           | the trace action for this trace                                   | [TraceAction](/data-model/chaindata/#traceaction) |
| result           | the trace result of this trace                                    | [TraceResult](/data-model/chaindata/#traceresult) |
| articulatedTrace | human readable version of the trace action input data             | [Function](/data-model/other/#function)           |
| compressedTrace  | a compressed string version of the articulated trace (calculated) | string                                            |

### Notes

Traces and TraceActions, when produced during a self-destruct, export different fields when rendered in JSON. In CSV and TXT output, these fields change thier meaning while retaining the header of the original fields. The following table describes these differences:

Fields that change during self-destruct transaction:

| Field         | When rendered in JSON | When rendered in csv/txt |
| ------------- | --------------------- | ------------------------ |
| Action.From   |                       | Action.Address           |
| Action::To    |                       | Action.RefundAddress     |
| Action::Value |                       | Action.Balance           |
|               | Action.RefundAddress  |                          |
|               | Action.Balance        |                          |

## TraceAction

Other than the first trace which is the trace of the transaction itself, traces represent calls
into smart contracts. Because of this, `trace actions` closely resemble the fields of the
[transaction](#transactions).

The following commands produce and manage TraceActions:

- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra blocks](/chifra/chaindata/#chifra-blocks)

TraceActions consist of the following fields:

| Field          | Description                                                                | Type    |
| -------------- | -------------------------------------------------------------------------- | ------- |
| from           | address from which the trace was sent                                      | address |
| to             | address to which the trace was sent                                        | address |
| gas            | the maximum number of gas allowed for this trace                           | gas     |
| input          | an encoded version of the function call                                    | bytes   |
| callType       | the type of call                                                           | string  |
| refundAddress  | if the call type is self-destruct, the address to which the refund is sent | address |
| rewardType     | the type of reward                                                         | string  |
| value          | the value (in wei) of this trace action                                    | wei     |
| ether          | if --ether is specified, the value in ether (calculated)                   | ether   |
| selfDestructed | `true` if the contract self-destructed, `false` otherwise                  | address |
| balance        | if self-destructed, the balance of the contract at that time               | wei     |
| balanceEth     | if --ether is specified, the balance in ether (calculated)                 | ether   |

## TraceResult

As mentioned above, other than the first trace, traces represent calls into other smart contracts.
Because of this, the trace results closely resembles the fields of the [receipt](#receipts).

The following commands produce and manage TraceResults:

- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra blocks](/chifra/chaindata/#chifra-blocks)

TraceResults consist of the following fields:

| Field   | Description                                                                    | Type    |
| ------- | ------------------------------------------------------------------------------ | ------- |
| address | address of new contract, if any                                                | address |
| code    | if this trace is creating a new smart contract, the byte code of that contract | bytes   |
| gasUsed | the amount of gas used by this trace                                           | gas     |
| output  | the result of the call of this trace                                           | bytes   |

## TraceCount

`chifra trace --count` returns the number of traces the given transaction.

The following commands produce and manage TraceCounts:

- [chifra traces](/chifra/chaindata/#chifra-traces)

TraceCounts consist of the following fields:

| Field            | Description                             | Type      |
| ---------------- | --------------------------------------- | --------- |
| blockNumber      | the block number                        | blknum    |
| transactionIndex | the transaction index                   | txnum     |
| transactionHash  | the transaction's hash                  | hash      |
| timestamp        | the timestamp of the block              | timestamp |
| date             | the timestamp as a date (calculated)    | datetime  |
| tracesCnt        | the number of traces in the transaction | uint64    |

## TraceFilter

The `traceFilter` is an internal data structure used to query using the `chifra traces --filter` command. Its use may, in the future, be expanded for other use cases. Note that all fields are optional, but not all may be empty at the same time.

The following commands produce and manage TraceFilters:

- [chifra traces](/chifra/chaindata/#chifra-traces)

TraceFilters consist of the following fields:

| Field       | Description                                                    | Type    |
| ----------- | -------------------------------------------------------------- | ------- |
| fromBlock   | the first block to include in the queried list of traces.      | blknum  |
| toBlock     | the last block to include in the queried list of traces.       | blknum  |
| fromAddress | if included, only traces `from` this address will be included. | address |
| toAddress   | if included, only traces `to` this address will be included.   | address |
| after       | only traces after this many traces are included.               | uint64  |
| count       | only this many traces are included.                            | uint64  |

## BlockCount

`chifra blocks --count` returns the number of various types of data in a block. For example, `transactionCnt` is the number of transactions in the block, and so on.

The following commands produce and manage BlockCounts:

- [chifra blocks](/chifra/chaindata/#chifra-blocks)

BlockCounts consist of the following fields:

| Field           | Description                                    | Type      |
| --------------- | ---------------------------------------------- | --------- |
| blockNumber     | the block's block number                       | blknum    |
| timestamp       | the timestamp of the block                     | timestamp |
| date            | the timestamp as a date (calculated)           | datetime  |
| transactionsCnt | the number transactions in the block           | uint64    |
| unclesCnt       | the number of uncles in the block              | uint64    |
| logsCnt         | the number of logs in the block                | uint64    |
| tracesCnt       | the number of traces in the block              | uint64    |
| withdrawalsCnt  | the number of withdrawals in the block         | uint64    |
| addressCnt      | the number of address appearances in the block | uint64    |

## NamedBlock

Left to its own devices, the blockchain would try to convince us that only hashes and bytes are
important, but being human beings we know that this is not true. TrueBlocks `articulates` various
types of data with [chifra names](/chifra/accounts/#chifra-names) detailing the names for
addresses, `-articulate` describing the Functions and Events of a transaction, and
[chifra when](/chifra/chaindata/#chifra-when) describing dated blocks. Dated blocks assign a
human-readable date to blocks given block numbers or timestamps and visa versa.

The following commands produce and manage NamedBlocks:

- [chifra when](/chifra/chaindata/#chifra-when)

NamedBlocks consist of the following fields:

| Field       | Description                          | Type      |
| ----------- | ------------------------------------ | --------- |
| blockNumber | the number of the block              | blknum    |
| timestamp   | the Unix timestamp of the block      | timestamp |
| date        | the timestamp as a date (calculated) | datetime  |
| name        | an optional name for the block       | string    |
| description | an optional description of the block | string    |

## Timestamp

Shows the blockNumber, timestamp and difference in seconds between blocks found in the timestamp database.

The following commands produce and manage Timestamps:

- [chifra when](/chifra/chaindata/#chifra-when)

Timestamps consist of the following fields:

| Field       | Description                                | Type      |
| ----------- | ------------------------------------------ | --------- |
| blockNumber | the number of the block                    | blknum    |
| timestamp   | the Unix timestamp of the block            | timestamp |
| date        | the timestamp as a date (calculated)       | datetime  |
| diff        | the number of seconds since the last block | int64     |

## TimestampCount

Shows the number of timestamps in the timestamps database.

The following commands produce and manage TimestampCounts:

- [chifra when](/chifra/chaindata/#chifra-when)

TimestampCounts consist of the following fields:

| Field | Description                                         | Type   |
| ----- | --------------------------------------------------- | ------ |
| count | the number of timestamps in the timestamps database | uint64 |

## LightBlock

`chifra blocks --hashes` returns top level data specified block with only
the hashes of the block's transactions.

The following commands produce and manage LightBlocks:

- [chifra blocks](/chifra/chaindata/#chifra-blocks)

LightBlocks consist of the following fields:

| Field         | Description                                                   | Type                                              |
| ------------- | ------------------------------------------------------------- | ------------------------------------------------- |
| gasLimit      | the system-wide maximum amount of gas permitted in this block | gas                                               |
| hash          | the hash of the current block                                 | hash                                              |
| blockNumber   | the number of the block                                       | blknum                                            |
| parentHash    | hash of previous block                                        | hash                                              |
| miner         | address of block's winning miner                              | address                                           |
| difficulty    | the computational difficulty at this block                    | value                                             |
| timestamp     | the Unix timestamp of the object                              | timestamp                                         |
| date          | the timestamp as a date (calculated)                          | datetime                                          |
| transactions  | a possibly empty array of transaction hashes                  | string[]                                          |
| baseFeePerGas | the base fee for this block                                   | gas                                               |
| uncles        | a possibly empty array of uncle hashes                        | hash[]                                            |
| withdrawals   | a possibly empty array of withdrawals (post Shanghai)         | [Withdrawal[]](/data-model/chaindata/#withdrawal) |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                            | Notes          |
| --------- | -------------------------------------- | -------------- |
| address   | an '0x'-prefixed 20-byte hex string    | lowercase      |
| blknum    | an alias for a uint64                  |                |
| bool      | either `true`, `false`, `1`, or `0`    |                |
| bytes     | an arbitrarily long string of bytes    |                |
| datetime  | a JSON formatted date                  | as a string    |
| ether     | a big number float                     | as a string    |
| gas       | a 64-bit unsigned integer              |                |
| hash      | an '0x'-prefixed 32-byte hex string    | lowercase      |
| int64     | a 64-bit signed integer                |                |
| lognum    | an alias for a uint64                  |                |
| string    | a normal character string              |                |
| timestamp | a 64-bit unsigned integer              | Unix timestamp |
| topic     | an '0x'-prefixed 32-byte hex string    | lowercase      |
| txnum     | an alias for a uint64                  |                |
| uint256   | a 256-bit unsigned integer             |                |
| uint64    | a 64-bit unsigned integer              |                |
| value     | an alias for a 64-bit unsigned integer |                |
| wei       | an unsigned big number                 | as a string    |

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
