---
title: "Other"
description: ""
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
menu:
  data:
    parent: "collections"
weight: 1800
toc: true
---

<!-- markdownlint-disable MD033 MD036 MD041 -->
The following commands provide useful miscellaneous tools:

- `chifra explore` a quick way to open a blockchain explorer,
- `ethslurp` an older tool that lets you call data from Etherscan. (This has issues of centralization and data quality, see explanation in its section).

Note: some of these tools, such as `ethslurp`, require an Etherscan key. [Follow these instructions
to add a key to your config](/docs/install/install-core/#3-update-the-configs-for-your-rpc-and-api-keys).

Each data structure is created by one or more tools which are detailed below.

## Function

<!-- markdownlint-disable MD033 MD036 MD041 -->
ABI files are derived from the Solidity source code of a smart contract by extracting the canonical
function and event signatures in a JSON structure. The function signatures are hashed (using
keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the
blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the
human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks
commands provide an `--articulate` option. See the commands themselves for more information.

The following commands produce and manage Functions:

- [chifra abis](/chifra/accounts/#chifra-abis)
- [chifra export](/chifra/accounts/#chifra-export)

Functions consist of the following fields:

| Field     | Description                                             | Type                                        |
| --------- | ------------------------------------------------------- | ------------------------------------------- |
| name      | the name of the interface                               | string                                      |
| type      | the type of the interface, either 'event' or 'function' | string                                      |
| signature | the canonical signature of the interface                | string                                      |
| encoding  | the signature encoded with keccak                       | string                                      |
| inputs    | the input parameters to the function, if any            | [Parameter[]](/data-model/other/#parameter) |
| outputs   | the output parameters to the function, if any           | [Parameter[]](/data-model/other/#parameter) |

## Parameter

<!-- markdownlint-disable MD033 MD036 MD041 -->
Parameters are a constituent part of a [Function or Event](/data-model/accounts/#function). The
parameters of a function are each individual value passed into the function. Along with the
function's name, the parameters types (once canonicalized) are used to create a function's four
byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because
we use them as part of the ABI decoding and the `--articulate` process to conver the blockchain's
bytes into human-readable text.

The following commands produce and manage Parameters:

- [chifra abis](/chifra/accounts/#chifra-abis)
- [chifra export](/chifra/accounts/#chifra-export)

Parameters consist of the following fields:

| Field        | Description                                                 | Type                                        |
| ------------ | ----------------------------------------------------------- | ------------------------------------------- |
| type         | the type of this parameter                                  | string                                      |
| name         | the name of this parameter                                  | string                                      |
| strDefault   | the default value of this parameter, if any                 | string                                      |
| indexed      | `true` if this parameter is indexed                         | bool                                        |
| internalType | for composite types, the internal type of the parameter     | string                                      |
| components   | for composite types, the parameters making up the composite | [Parameter[]](/data-model/other/#parameter) |

## Slurp

<!-- markdownlint-disable MD033 MD036 MD041 -->
THIS SHOULD BE ETHERSCAN DATA RELATED, BUT IT'S NOT TIED IN, SO IT DOESN'T DO ANYTHING
The `traceFilter` is an internal data structure used to query using the `chifra traces --filter` command. Its use may, in the future, be expanded for other use cases. Note that all fields are optional, but not all may be empty at the same time.

The following commands produce and manage Slurps:

- [chifra slurp](/chifra/other/#chifra-slurp)

Slurps consist of the following fields:

| Field            | Description                                                                                           | Type                                    |
| ---------------- | ----------------------------------------------------------------------------------------------------- | --------------------------------------- |
| hash             | the hash of the transaction                                                                           | hash                                    |
| blockHash        | the hash of the block containing this transaction                                                     | hash                                    |
| blockNumber      | the number of the block                                                                               | blknum                                  |
| transactionIndex | the zero-indexed position of the transaction in the block                                             | blknum                                  |
| nonce            | sequence number of the transactions sent by the sender                                                | uint64                                  |
| timestamp        | the Unix timestamp of the object                                                                      | timestamp                               |
| date             | a calculated field -- the date of the object                                                          | datetime                                |
| from             | address from which the transaction was sent                                                           | address                                 |
| to               | address to which the transaction was sent                                                             | address                                 |
| value            | the amount of wei sent with this transactions                                                         | wei                                     |
| gas              | the maximum number of gas allowed for this transaction                                                | gas                                     |
| gasPrice         | the number of wei per unit of gas the sender is willing to spend                                      | gas                                     |
| input            | byte data either containing a message or funcational data for a smart contracts. See the --articulate | bytes                                   |
| hasToken         | `true` if the transaction is token related, `false` otherwise                                         | bool                                    |
| articulatedTx    |                                                                                                       | [Function](/data-model/other/#function) |
| compressedTx     | truncated, more readable version of the articulation                                                  | string                                  |
| isError          | `true` if the transaction ended in error, `false` otherwise                                           | bool                                    |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                         | Notes          |
| --------- | ----------------------------------- | -------------- |
| address   | an '0x'-prefixed 20-byte hex string | lowercase      |
| blknum    | an alias for a uint64               |                |
| bool      | either `true`, `false`, `1`, or `0` |                |
| bytes     | an arbitrarily long string of bytes |                |
| datetime  | a JSON formatted date               | as a string    |
| gas       | an unsigned big number              | as a string    |
| hash      | an '0x'-prefixed 32-byte hex string | lowercase      |
| string    | a normal character string           |                |
| timestamp | a 64-bit unsigned integer           | Unix timestamp |
| uint64    | a 64-bit unsigned integer           |                |
| wei       | an unsigned big number              | as a string    |
