---
title: "Other"
description: ""
lead: ""
date: 2021-10-03T20:13:02
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
weight: 1800
toc: true
---

These commands call some useful miscellaneous tools:
* `chifra quotes`, a rudimentary pricing tool,
* `chifra explore` a quick way to open a blockchain explorer,
* `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of centralization and data quality, see explanation in its section).

{{< alert icon="👉" title="note about keys"
text="Note: some of these tools, like `ethslurp`, require an EtherScan key." >}}
[Follow these instructions to add a key to your config](/docs/install/install-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys)

_Each data structure is created by one or more tools which are detailed below_

## Function

ABI files are derived from the Solidity source code of a smart contract by extracting the canonical function and event signatures in a JSON structure. The function signatures are hashed (using keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks commands provide an `--articulate` option. See the commands themselves for more information.

The following commands produce and manage functions:

| Tools                                                 |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |
| [chifra abis](/docs/chifra/accounts/#chifra-abis)     | fetches the ABI for a smart contract                         |

Function data is made of the following data fields:

| Field     | Description                                             | Type            |
| --------- | ------------------------------------------------------- | --------------- |
| name      | the name of the interface                               | string          |
| type      | the type of the interface, either 'event' or 'function' | string          |
| signature | the canonical signature of the interface                | string          |
| encoding  | the signature encoded with keccak                       | string          |
| inputs    | the input parameters to the function, if any            | CParameterArray |
| outputs   | the output parameters to the function, if any           | CParameterArray |


## Parameter

Parameters are a constituent part of a [Function or Event](/data-model/accounts/#function). The parameters of a function are each individual value passed into the function. Along with the function's name, the parameters types (once canonicalized) are used to create a function's four byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because we use them as part of the ABI decoding and the `--articulate` process to conver the blockchain's bytes into human-readable text.

The following commands produce and manage parameters:

| Tools                                                 |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |
| [chifra abis](/docs/chifra/accounts/#chifra-abis)     | fetches the ABI for a smart contract                         |

Parameter data is made of the following data fields:

| Field        | Description                                                 | Type            |
| ------------ | ----------------------------------------------------------- | --------------- |
| type         | the type of this parameter                                  | string          |
| name         | the name of this parameter                                  | string          |
| str_default  | the default value of this parameter, if any                 | string          |
| indexed      | `true` if this parameter is indexed                         | bool            |
| internalType | for composite types, the internal type of the parameter     | string          |
| components   | for composite types, the parameters making up the composite | CParameterArray |


## Quote

Prior to sources of on-chain prices such as UniSwap, TrueBlocks would pull US dollar to Ether prices from the Poloniex price API. This data structure reports on those queries. Note that this tool has been deprecated.

The following commands produce and manage quotes:

| Tools                                              |                                                                           |
| -------------------------------------------------- | ------------------------------------------------------------------------- |
| [chifra quotes](/docs/chifra/other/#chifra-quotes) | Freshen or display Ethereum price data.<br/>This tool has been deprecated |

Quote data is made of the following data fields:

| Field     | Description                                           | Type      |
| --------- | ----------------------------------------------------- | --------- |
| timestamp | the timestamp of this quote                           | timestamp |
| close     | price of the asset in US dollars                      | double    |
| date      | the date of the associated timestamp                  | date      |
| name      | the name of the block for some named 'special' blocks | string    |


## Base types

This documentation mentions the following basic data types.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| date      | a JSON formatted date                           | as a string    |
| double    | a floating point number of double precision     |                |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | Unix timestamp |
