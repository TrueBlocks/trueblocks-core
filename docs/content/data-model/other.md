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
- `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of centralization and data quality, see explanation in its section).

Note: some of these tools, such as `ethslurp`, require an EtherScan key. [Follow these instructions
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

## Base types

This documentation mentions the following basic data types.

| Type   | Description                         | Notes |
| ------ | ----------------------------------- | ----- |
| bool   | either `true`, `false`, `1`, or `0` |       |
| string | a normal character string           |       |
