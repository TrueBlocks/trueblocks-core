---
title: "Admin"
description: ""
lead: ""
date: 2022-09-19T21:00:15
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

Tools in the Admin category produce data related to scraping the chain, producing the indexes, and querying the status of the system. Additional data related to sharing the indexes via IPFS and pinning the same are also produced by tools in this category.

## Status

The [chifra status](/docs/chifra/admin/#chifra-status) tool reports on the state (and size) of the various TrueBlocks local binary caches. TrueBlocks produces nine difference caches: `abis`, `blocks`, `monitors`, `names`, `objs`, `recons`, `slurps`, `traces`, `txs`. In general practice, these caches may take up a few GB of hard drive space, however, for very popular smart contract the size of the caches may grow rather large. Keep an eye on it.

The following commands produce and manage status:

| Tools                                              |                                               |
| -------------------------------------------------- | --------------------------------------------- |
| [chifra status](/docs/chifra/admin/#chifra-status) | report on the status of the TrueBlocks system |

Status data is made of the following data fields:

| Field             | Description                                              | Type           |
| ----------------- | -------------------------------------------------------- | -------------- |
| clientVersion     | the version string as reported by the rpcProvider        | string         |
| clientIds         | netword_id and chain_id from the rpcProvider             | string         |
| trueblocksVersion | the TrueBlocks version string                            | string         |
| rpcProvider       | the current rpcProvider                                  | string         |
| configPath        | the path to config files                                 | string         |
| cachePath         | the path to the local binary caches                      | string         |
| indexPath         | the path to the local binary indexes                     | string         |
| host              | the host portion of the local API server                 | string         |
| isTesting         | `true` if the server is running in test mode             | bool           |
| isApi             | `true` if the server is running in API mode              | bool           |
| isScraping        | `true` if the index scraper is running                   | bool           |
| isArchive         | `true` if the rpcProvider is an archive node             | bool           |
| isTracing         | `true` if the rpcProvider provides Parity traces         | bool           |
| hasEskey          | `true` if an EtherScan key is present                    | bool           |
| hasPinkey         | `true` if a Pinata API key is present                    | bool           |
| ts                | the timestamp when this status data was produced         | timestamp      |
| chains            |                                                          | CChainArray    |
| caches            | a collection of information concerning the binary caches | CCachePtrArray |


## Cache

The [chifra status <type>](/docs/chifra/admin/#chifra-status) reports on the binary caches. Those reports come in the form of the Cache data type. Each cache data object may carry unique information for the given cache. See the source code for more information.

The following commands produce and manage caches:

| Tools                                              |                                               |
| -------------------------------------------------- | --------------------------------------------- |
| [chifra status](/docs/chifra/admin/#chifra-status) | report on the status of the TrueBlocks system |

Cache data is made of the following data fields:

| Field       | Description                                             | Type             |
| ----------- | ------------------------------------------------------- | ---------------- |
| type        | the type of the cache (one of the nine different types) | string           |
| path        | the physical path to the cache on the hard drive        | string           |
| nFiles      | the number of files in the cache                        | uint64           |
| nFolders    | the number of subfolders in the cache                   | uint64           |
| sizeInBytes | the size of the cache in bytes                          | uint64           |
| items       | an array of cache items                                 | CCacheEntryArray |


## PinnedChunk

The TrueBlocks index scraper periodically creates a chunked portion of the index so that it can be more easily stored in a content-addresable data store such as IPFS. We call these periodically-created chunks, PinnedChunks. The format of said item is described here. A pinned chunk is effectively a relational table relating all of the addresses appearing in the chunk with a list of appearances appearing in the chunk.

The following commands produce and manage pinnedchunks:

| Tools                                              |                                                                 |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [chifra scrape](/docs/chifra/admin/#chifra-scrape) | scan the chain and update the TrueBlocks index of appearances   |
| [chifra chunks](/docs/chifra/admin/#chifra-chunks) | manage pinned index of appearances and associated Bloom filters |

Pinnedchunk data is made of the following data fields:

| Field     | Description                                                 | Type     |
| --------- | ----------------------------------------------------------- | -------- |
| range     | for each chunk, the range of blocks contained in that chunk | string   |
| bloomHash | the IPFS hash of the bloom filter at that range             | ipfshash |
| indexHash | the IPFS hash of the index chunk at that range              | ipfshash |
| firstApp  | the first appearance in the chunk                           | blknum   |
| latestApp | the latest appearance in the chunk                          | blknum   |


## Manifest

The Manifest details the index of appearance's PinnedChunks. Each record in the Manifest details the block range represented by the chunk as well as the IPFS hash of the index chunk along with the associated IPFS hash for the Bloom filter of the chunk. The manifest itself is also pushed to IPFS and the IPFS of the hash of the manifest is published periodically to the Unchained Index smart contract.

The following commands produce and manage manifests:

| Tools                                              |                                                                 |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [chifra scrape](/docs/chifra/admin/#chifra-scrape) | scan the chain and update the TrueBlocks index of appearances   |
| [chifra chunks](/docs/chifra/admin/#chifra-chunks) | manage pinned index of appearances and associated Bloom filters |

Manifest data is made of the following data fields:

| Field     | Description                                                           | Type              |
| --------- | --------------------------------------------------------------------- | ----------------- |
| version   | the version string hashed into the chunk data                         | string            |
| chain     | the chain to which this manifest belongs                              | string            |
| schemas   | IPFS cid of file describing the schemas for the various databases     | ipfshash          |
| databases | IPFS cid of file containing CIDs for the various databases            | ipfshash          |
| chunks    | a list of the IPFS hashes of all of the chunks in the unchained index | CPinnedChunkArray |


## Chain


| Field          | Description                                                      | Type   |
| -------------- | ---------------------------------------------------------------- | ------ |
| chain          | The common name of the chain                                     | string |
| chainId        | The chain id as reported by the RPC                              | uint64 |
| symbol         | The symbol of the base currency on the chain                     | string |
| rpcProvider    | A valid RPC provider for the chain                               | string |
| apiProvider    | A valid API provider for the explorer                            | string |
| remoteExplorer | A remote explorer for the chain such as EtherScan                | string |
| localExplorer  | The local explorer for the chain (typically TrueBlocks Explorer) | string |
| ipfsGateway    | An IPFS gateway for pinning the index if enabled                 | string |

## CacheEntry


| Field   | Description | Type    |
| ------- | --- | ------- |
| address |     | address |
| name    |     | string  |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| address   | a 20-byte hexadecimal string starting with '0x' | lowercase      |
| blknum    | an alias for a uint64                           |                |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| ipfshash  | a multi-hash produced by IPFS                   | mixed-case     |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | Unix timestamp |
| uint64    | a 64-bit unsigned integer                       |                |
