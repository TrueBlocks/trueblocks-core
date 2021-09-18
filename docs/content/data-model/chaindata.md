---
title: "Chain Data"
description: ""
lead: ""
date: 2021-09-17T22:46:26
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

_These fields describe the output of various TrueBlocks blockchain queries.

_Each data structure is created by one or more tools which are detailed below_

## Block

`chifra blocks` returns top level data specified block. You can also include
an array for the blocks' transactions.

### How to get blocks

- **CLI**:
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-blocks)
- **API**:
  - [Calls to `/blocks`](/api#operation/chaindata-blocks)

Below is a list of the data fields for blocks. Following that are the commands that produce or manage blocks.  
### Fields
| Field         | Description                                                   | Type              |
| ------------- | ------------------------------------------------------------- | ----------------- |
| gasLimit      | the system-wide maximum amount of gas permitted in this block | gas               |
| hash          | the hash of the current block                                 | hash              |
| blockNumber   | the number of the block                                       | blknum            |
| parentHash    | hash of previous block                                        | hash              |
| miner         | Address of block's winning miner                              | address           |
| difficulty    | the computational difficulty at this block                    | uint64            |
| timestamp     | the unix timestamp of the object                              | timestamp         |
| transactions  | a possibly empty array of transactions or transaction hashes  | CTransactionArray |
| baseFeePerGas | the base fee for this block                                   | wei               |
| finalized     | flag indicating the system considers this data final          | bool              |

## Transaction

Transactions represent eth transfers to and from other addresses.

Most of the fields that TrueBlocks returns are standard to all eth transaction.
However, one field is very interesting: `articulatedTx` provides a human readable output of the `input` field.

This is a very powerful way to understand the story behind a smart contract.

### How to get transactions

- **CLI**:
  - run `chifra transactions <txn_id>`.
  - Use the `--articulate` option to turn the `input` field human readable.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-transactions)
- **API**:
  - [Calls to `/transactions`](/api#operation/chaindata-transactions)

Below is a list of the data fields for transactions. Following that are the commands that produce or manage transactions.  
### Fields
| Field            | Description                                                                                           | Type                 |
| ---------------- | ----------------------------------------------------------------------------------------------------- | -------------------- |
| hash             | The hash of the transaction                                                                           | hash                 |
| gasPrice         | the number of wei per unit of gas the sender is willing to spend                                      | gas                  |
| blockHash        | The hash of the block containing this transaction                                                     | hash                 |
| blockNumber      | the number of the block                                                                               | blknum               |
| transactionIndex | the zero-indexed position of the transaction in the block                                             | blknum               |
| nonce            | sequence number of the transactions sent by the sender                                                | uint64               |
| timestamp        | the unix timestamp of the object                                                                      | timestamp            |
| from             | address from which the transaction was sent                                                           | address              |
| to               | address to which the transaction was sent                                                             | address              |
| value            | the amount of wei sent with this transactions                                                         | wei                  |
| gas              | the maximum number of gas allowed for this transaction                                                | gas                  |
| input            | byte data either containing a message or funcational data for a smart contracts. See the --articulate | bytes                |
| receipt          |                                                                                                       | CReceipt             |
| statements       | array of reconciliations                                                                              | CReconciliationArray |
| articulatedTx    |                                                                                                       | CFunction            |
| compressedTx     | truncated, more readable version of the articulation                                                  | string               |
| hasToken         | `true` if the transaction is token related, `false` otherwise                                         | uint8                |
| finalized        | flag indicating the system considers this data final                                                  | bool                 |

## Receipt

Receipts record the amount of gas used for a transaction among other things. If the transaction succeeded, a receipt might also have logs.

If the `to` address of a transaction is `0x0`, the `input` data is considered to be the the source code (byte code) of a smart contract. In this case, if the creation of the contract succeeds, the `contractAddress` field of the receipt carries the address of the newly created contract.

### How to get receipts

- **CLI**:
  - run `chifra receipts <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-receipts)
- **API**:
  - [Calls to `/receipts`](/api#operation/chaindata-receipts)

Below is a list of the data fields for receipts. Following that are the commands that produce or manage receipts.  
### Fields
| Field           | Description                                                                | Type           |
| --------------- | -------------------------------------------------------------------------- | -------------- |
| status          | `1` on transaction suceess, `null` if tx preceeds Byzantium, `0` otherwise | uint32         |
| contractAddress | the address of the newly created contract, if any                          | address        |
| gasUsed         | the amount of gas actually used by the transaction                         | gas            |
| logs            | a possibly empty array of logs                                             | CLogEntryArray |

## Log

Logs appear in a possibly empty array in the transaction's receipt. They are only created if the underlying transaction suceeded. In the case where the transaction failed, no logs will appear in the receipt. Logs are only ever generated during transactions whose `to` address is a smart contract.

### How to get logs

- **CLI**:
  - run `chifra logs <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-logs)
- **API**:
  - [Calls to `/logs`](/api#operation/chaindata-logs)

Below is a list of the data fields for logs. Following that are the commands that produce or manage logs.  
### Fields
| Field            | Description                                                                                       | Type        |
| ---------------- | ------------------------------------------------------------------------------------------------- | ----------- |
| blockNumber      | the number of the block                                                                           | blknum      |
| transactionIndex | the zero-indexed position of the transaction in the block                                         | blknum      |
| address          | the smart contract that emitted this log                                                          | address     |
| logIndex         | the zero-indexed position of this log relative to the block                                       | blknum      |
| topics           | The first topic hashes event signature of the log, up to 3 additional index parameters may appear | CTopicArray |
| data             | any remaining un-indexed parameters to the event                                                  | bytes       |
| articulatedLog   | a human-readable version of the topic and data fields                                             | CFunction   |
| compressedLog    | a truncated, more readable version of the articulation                                            | string      |

## Trace

The deepest layer of the Ethereum data is the trace. Every transaction has at least one trace which is itself a record of the transaction. If the `to` address of the transaction is a smart contract, other traces may appear, if, for example, that smart contract calls other smart contracts.

Traces may be arbitrarily deep (up to the gasLimit) and ultimately represent a tree of function calls. Some transactions have 100s of traces. The format of the trace is similar to the transaction itself have a trace `action` (which contains `from`, `to`, `value` like the transaction) and the trace `result` (containing `gasUsed` like the receipt).

### How to get traces

- **CLI**:
  - run `chifra traces <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-traces)
- **API**:
  - [Calls to `/traces`](/api#operation/chaindata-traces)

Below is a list of the data fields for traces. Following that are the commands that produce or manage traces.  
### Fields
| Field            | Description                                               | Type         |
| ---------------- | --------------------------------------------------------- | ------------ |
| blockHash        | The hash of the block containing this trace               | hash         |
| blockNumber      | the number of the block                                   | blknum       |
| transactionHash  | the transaction's hash containing this trace              | hash         |
| transactionIndex | the zero-indexed position of the transaction in the block | blknum       |
| traceAddress     | a particular trace's address in the trace tree            | CStringArray |
| subtraces        | the number of children traces that the trace hash         | uint64       |
| type             | the type of the trace                                     | string       |
| action           | the trace action for this trace                           | CTraceAction |
| result           | the trace result of this trace                            | CTraceResult |
| articulatedTrace | human readable version of the trace action input data     | CFunction    |
| compressedTrace  | a compressed string version of the articulated trace      | string       |

## TraceAction

Other than the first trace which is the trace of the transaction itself, traces represent calls into smart contracts. Because of this, `trace actions` closely resemble the fields of the [transaction](#transactions).

Below is a list of the data fields for traceactions. Following that are the commands that produce or manage traceactions.  
### Fields
| Field         | Description                                                                | Type    |
| ------------- | -------------------------------------------------------------------------- | ------- |
| from          | address from which the trace was sent                                      | address |
| to            | address to which the trace was sent                                        | address |
| gas           | the maximum number of gas allowed for this trace                           | gas     |
| input         | an encoded version of the function call                                    | bytes   |
| callType      | the type of call                                                           | string  |
| refundAddress | if the call type is self-destruct, the address to which the refund is sent | address |

## TraceResult

As mentioned above, other than the first trace, traces represent calls into other smart contracts. Because of this, the trace results closely resembles the fields of the [receipt](#receipts).

Below is a list of the data fields for traceresults. Following that are the commands that produce or manage traceresults.  
### Fields
| Field       | Description                                                                    | Type    |
| ----------- | ------------------------------------------------------------------------------ | ------- |
| newContract | Address of new contract, if any                                                | address |
| code        | if this trace is creating a new smart contract, the byte code of that contract | bytes   |
| gasUsed     | the amount of gas used by this trace                                           | gas     |
| output      | the result of the call of this trace                                           | bytes   |

## DatedBlock

### How to get when blocks

- **CLI**:
  - run `chifra when <block_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-when)
- **API**:
  - [Calls to `/when`](/api#operation/chaindata-when)

Below is a list of the data fields for datedblocks. Following that are the commands that produce or manage datedblocks.  
### Fields
| Field       | Description                         | Type      |
| ----------- | ----------------------------------- | --------- |
| blockNumber | the number of the block             | blknum    |
| timestamp   | the unix timestamp of the block     | timestamp |
| date        | Human readable version of timestamp | date      |

## Base types

The above documentation mentions common data types as detailed below.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| address   | a 20-byte hexidecimal string starting with '0x' | lowercase      |
| blknum    | an alias for a uint64                           |                |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| bytes     | an arbitrarily long string of bytes             |                |
| date      | a JSON formatted date                           | as a string    |
| double    | a floating point number of double precision     |                |
| gas       | an unsigned big number                          | as a string    |
| hash      | a 32-byte hexidecimal string starting with '0x' | lowercase      |
| int256    | a signed big number                             | as a string    |
| ipfshash  | a multi-hash produced by IPFS                   | mixed-case     |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | unix timestamp |
| uint32    | a 32-bit unsigned integer                       |                |
| uint64    | a 64-bit unsigned integer                       |                |
| uint8     | an alias for the boolean type                   |                |
| wei       | an unsigned big number                          | as a string    |
