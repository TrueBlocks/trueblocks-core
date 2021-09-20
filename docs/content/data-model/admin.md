---
title: "Admin"
description: ""
lead: ""
date: 2021-09-18T09:10:08
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

Below is a list of the data fields for status. Following that are the commands that produce or manage status.

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

---

## PinnedChunk

<!-- TEXT ABOUT PINNED CHUNKS -->

### How to get pinnedchunks

- **CLI**:
  - Run `chifra pins <options>`
  - [See the command's documentation](/docs/chifra/admin/#chifra-pins)
- **API**:
  - [Calls to `/pins`](/api#operation/admin-pins)

Below is a list of the data fields for pinnedchunks. Following that are the commands that produce or manage pinnedchunks.

| Field     | Description                                                 | Type     |
| --------- | ----------------------------------------------------------- | -------- |
| fileName  | for each chunk, the range of blocks contained in that chunk | string   |
| bloomHash | the IPFS hash of the bloom filter at that range             | ipfshash |
| indexHash | the IPFS hash of the index chunk at that range              | ipfshash |

---

## Manifest


Below is a list of the data fields for manifests. Following that are the commands that produce or manage manifests.

| Field       | Description                                                     | Type              |
| ----------- | --------------------------------------------------------------- | ----------------- |
| indexFormat | IPFS cid of file describing the file format of an index chunk   | string            |
| bloomFormat | IPFS cid of file describing the assoicated bloom filter         | string            |
| firstPin    | the block number of the first pin in the manifest (always zero) | blknum            |
| lastPin     | the most recent block included in this manifest                 | blknum            |
| pins        | a list of all the pins in the unchained index                   | CPinnedChunkArray |

---

## Base types

The above documentation mentions the following basic data types.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| blknum    | an alias for a uint64                           |                |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| ipfshash  | a multi-hash produced by IPFS                   | mixed-case     |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | unix timestamp |
