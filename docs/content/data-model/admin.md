---
title: "Admin"
description: ""
lead: ""
date: 2021-09-16T11:03:44
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
weight: 1700
toc: true
---

The Admin component allows you to query the status of the TrueBlocks system. You may query the status; query for information about TrueBlocks caches; control the creation, sharing, and pinning of the TrueBlocks index of appearances; and even serve the data through an API.

_Each data structure is created by one or more tools which are detailed below_

## Status

<!-- TEXT ABOUT STATUS -->

### How to get status

- **CLI**:
  - Run `chifra status <options>`
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-transactions)
- **API**:
  - [Calls to `/transactions`](/api#operation/chaindata-transactions)

### Fields

| Field              | Description                                                         | Type      |
| ------------------ | ------------------------------------------------------------------- | --------- |
| client_version     | the version string as reported by the rpcProvider                   | string    |
| client_ids         | netword_id and chain_id from the rpcProvider                        | string    |
| trueblocks_version | the TrueBlocks version string                                       | string    |
| rpc_provider       | the current rpcProvider                                             | string    |
| balance_provider   | the current archive node balance provider (defaults to rpcProvider) | string    |
| config_path        | the path to config files                                            | string    |
| cache_path         | the path to the local binary caches                                 | string    |
| index_path         | the path to the local binary indexes                                | string    |
| host               | the host portion of the local API server                            | string    |
| is_testing         | `true` if the server is running in test mode                        | bool      |
| is_api             | `true` if the server is running in API mode                         | bool      |
| is_docker          | `true` if running under a docker image                              | bool      |
| is_scraping        | `true` if the index scraper is running                              | bool      |
| is_archive         | `true` if the rpcProvider is an archive node                        | bool      |
| is_tracing         | `true` if the rpcProvider provides Parity traces                    | bool      |
| has_eskey          | `true` if an EtherScan key is present                               | bool      |
| has_pinkey         | `true` if a Pinata API key is present                               | bool      |
| ts                 | the timestamp when this status data was produced                    | timestamp |

## PinnedChunk

<!-- TEXT ABOUT PINNED CHUNKS -->

### How to get pinnedchunks

- **CLI**:
  - Run `chifra pins <options>`
  - [See the command's documentation](/docs/chifra/admin/#chifra-pins)
- **API**:
  - [Calls to `/pins`](/api#operation/admin-pins)

### Fields

| Field     | Description                                                 | Type     |
| --------- | ----------------------------------------------------------- | -------- |
| fileName  | for each chunk, the range of blocks contained in that chunk | string   |
| bloomHash | the IPFS hash of the bloom filter at that range             | ipfshash |
| indexHash | the IPFS hash of the index chunk at that range              | ipfshash |

## Base types

In these docs, sometimes Trueblocks mentions a type format that is more
precise than the generic types, like "string" or "object".

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
