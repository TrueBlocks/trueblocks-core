---
title: "Other"
description: "Access to other and external data"
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
menu:
  data:
    parent: collections
weight: 51000
toc: true
---

The following commands provide useful miscellaneous tools:

- `chifra explore` a quick way to open a blockchain explorer,
- `ethslurp` an older tool that lets you call data from Etherscan. (This has issues of centralization and data quality, see explanation in its section).

Note: some of these tools, such as `ethslurp`, require an Etherscan key. [Follow these instructions
to add a key to your config](/docs/install/install-core/#3-update-the-configs-for-your-rpc-and-api-keys).

Each data structure is created by one or more tools which are detailed below.

## Abi

An ABI describes an Application Binary Interface -- in other words, the [Function](/data-model/other/#function)
and Event signatures for a given smart contract. Along with [Names](/data-model/accounts/#names) the
use of ABIs goes a very long way towards making your Ethereum data much more understandable.

Similar to names of addresses, ABI files are not available on-chain which means they must be
acquired somewhere. Unfortunately, the Ethereum community has not yet understood that Etherscan
is not a good place to store this very important information. For this reason, TrueBlocks uses
Etherscan to acquire ABI files and therefore one needs to get an Etherscan API key to use this
function.

The following commands produce and manage Abis:

- [chifra abis](/chifra/accounts/#chifra-abis)

Abis consist of the following fields:

| Field          | Description                                                         | Type                                      |
| -------------- | ------------------------------------------------------------------- | ----------------------------------------- |
| address        | the address for the ABI                                             | address                                   |
| name           | the filename of the ABI (likely the smart contract address)         | string                                    |
| path           | the folder holding the abi file                                     | string                                    |
| fileSize       | the size of this file on disc                                       | int64                                     |
| lastModDate    | the last update date of the file                                    | string                                    |
| isKnown        | true if this is the ABI for a known smart contract or protocol      | bool                                      |
| isEmpty        | true if the ABI could not be found (and won't be looked for again)  | bool                                      |
| nFunctions     | if verbose, the number of functions in the ABI                      | int64                                     |
| nEvents        | if verbose, the number of events in the ABI                         | int64                                     |
| hasConstructor | if verbose and the abi has a constructor, then `true`, else `false` | bool                                      |
| hasFallback    | if verbose and the abi has a fallback, then `true`, else `false`    | bool                                      |
| functions      | the functions for this address                                      | [Function[]](/data-model/other/#function) |

### Notes

See the [chifra abis](/chifra/accounts/#chifra-abis) command line for information about getting an Etherscan key.

## Function

ABI files are derived from the Solidity source code of a smart contract by extracting the canonical
function and event signatures in a JSON structure. The function signatures are hashed (using
keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the
blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the
human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks
commands provide an `--articulate` option. See the commands themselves for more information.

The following commands produce and manage Functions:

- [chifra abis](/chifra/accounts/#chifra-abis)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra transactions](/chifra/chaindata/#chifra-transactions)
- [chifra receipts](/chifra/chaindata/#chifra-receipts)
- [chifra logs](/chifra/chaindata/#chifra-logs)
- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra state](/chifra/chainstate/#chifra-state)
- [chifra slurp](/chifra/other/#chifra-slurp)

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

Parameters are a constituent part of a [Function or Event](/data-model/accounts/#function). The
parameters of a function are each individual value passed into the function. Along with the
function's name, the parameter types (once canonicalized) are used to create a function's four
byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because
we use them as part of the ABI decoding and the `--articulate` process to convert the blockchain's
bytes into human-readable text.

The following commands produce and manage Parameters:

- [chifra abis](/chifra/accounts/#chifra-abis)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra transactions](/chifra/chaindata/#chifra-transactions)
- [chifra receipts](/chifra/chaindata/#chifra-receipts)
- [chifra logs](/chifra/chaindata/#chifra-logs)
- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra state](/chifra/chainstate/#chifra-state)
- [chifra slurp](/chifra/other/#chifra-slurp)

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
| transactionIndex | the zero-indexed position of the transaction in the block                                             | txnum                                   |
| nonce            | sequence number of the transactions sent by the sender                                                | value                                   |
| timestamp        | the Unix timestamp of the object                                                                      | timestamp                               |
| date             | the timestamp as a date (calculated)                                                                  | datetime                                |
| from             | address from which the transaction was sent                                                           | address                                 |
| to               | address to which the transaction was sent                                                             | address                                 |
| value            | the amount of wei sent with this transactions                                                         | wei                                     |
| ether            | if --ether is specified, the value in ether (calculated)                                              | ether                                   |
| gas              | the maximum number of gas allowed for this transaction                                                | gas                                     |
| gasPrice         | the number of wei per unit of gas the sender is willing to spend                                      | gas                                     |
| input            | byte data either containing a message or funcational data for a smart contracts. See the --articulate | bytes                                   |
| hasToken         | `true` if the transaction is token related, `false` otherwise                                         | bool                                    |
| articulatedTx    | if present, the function that was called in the transaction                                           | [Function](/data-model/other/#function) |
| compressedTx     | truncated, more readable version of the articulation (calculated)                                     | string                                  |
| isError          | `true` if the transaction ended in error, `false` otherwise                                           | bool                                    |

## Message

The Message type is used in various places to return information about a command. For example, when using the `chifra names --autoname` feature in the SDK, a Message type is returned.

The following commands produce and manage Messages:

- [chifra blocks](/chifra/chaindata/#chifra-blocks)
- [chifra chunks](/chifra/admin/#chifra-chunks)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra logs](/chifra/chaindata/#chifra-logs)
- [chifra monitors](/chifra/accounts/#chifra-monitors)
- [chifra names](/chifra/accounts/#chifra-names)
- [chifra scrape](/chifra/admin/#chifra-scrape)
- [chifra state](/chifra/chainstate/#chifra-state)
- [chifra traces](/chifra/chaindata/#chifra-traces)
- [chifra transactions](/chifra/chaindata/#chifra-transactions)
- [chifra when](/chifra/chaindata/#chifra-when)

Messages consist of the following fields:

| Field | Description        | Type   |
| ----- | ------------------ | ------ |
| msg   | the message        | string |
| num   | a number if needed | int64  |

## Count

Shows the number of timestamps in the timestamps database.

The following commands produce and manage Counts:

- [chifra when](/chifra/chaindata/#chifra-when)
- [chifra chunks](/chifra/admin/#chifra-chunks)

Counts consist of the following fields:

| Field | Description                               | Type   |
| ----- | ----------------------------------------- | ------ |
| count | the number of items in the given database | uint64 |

## Destination



The following commands produce and manage Destinations:

- [chifra explore](/chifra/other/#chifra-explore)

Destinations consist of the following fields:

| Field    | Description                      | Type     |
| -------- | -------------------------------- | -------- |
| term     | the term used to produce the url | string   |
| termType | the type of the term             | DestType |
| url      | the url produced                 | string   |
| source   | the option that produced the url | string   |

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
| string    | a normal character string              |                |
| timestamp | a 64-bit unsigned integer              | Unix timestamp |
| txnum     | an alias for a uint64                  |                |
| uint64    | a 64-bit unsigned integer              |                |
| value     | an alias for a 64-bit unsigned integer |                |
| wei       | an unsigned big number                 | as a string    |

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
