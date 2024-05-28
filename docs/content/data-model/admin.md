---
title: "Admin"
description: "Control the scraper and build the index"
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
menu:
  data:
    parent: collections
weight: 41000
toc: true
---

The data models produced by the tools in the Admin category relate to scraping the chain, producing
the Unchained Index, and querying the configuration of the system. Additional data related to
sharing the indexes via IPFS and pinning the same are also produced by tools in this category.

Each data structure is created by one or more tools which are detailed below.

## Status

The `status` data model is a complex beast. It contains various information including a list of
registered chains, information about many of the internal binary caches maintained by `chifra`
as well as current status information about the system including version information for both
`chifra` and the node it's running against.

The following commands produce and manage Status:

- [chifra status](/chifra/admin/#chifra-status)

Status consist of the following fields:

| Field         | Description                                              | Type                                        |
| ------------- | -------------------------------------------------------- | ------------------------------------------- |
| cachePath     | the path to the local binary caches                      | string                                      |
| caches        | a collection of information concerning the binary caches | [CacheItem[]](/data-model/admin/#cacheitem) |
| chain         | the current chain                                        | string                                      |
| chainConfig   | the path to the chain configuration folder               | string                                      |
| clientVersion | the version string as reported by the rpcProvider        | string                                      |
| chainId       | the path to config files                                 | string                                      |
| hasEsKey      | `true` if an Etherscan key is present                    | bool                                        |
| hasPinKey     | `true` if a Pinata API key is present                    | bool                                        |
| indexPath     | the path to the local binary indexes                     | string                                      |
| isApi         | `true` if the server is running in API mode              | bool                                        |
| isArchive     | `true` if the rpcProvider is an archive node             | bool                                        |
| isTesting     | `true` if the server is running in test mode             | bool                                        |
| isTracing     | `true` if the rpcProvider provides Parity traces         | bool                                        |
| isScraping    | `true` if the scraper is running                         | bool                                        |
| networkId     | the network id as reported by the rpcProvider            | string                                      |
| progress      | the progress string of the system                        | string                                      |
| rootConfig    | the path to the root configuration folder                | string                                      |
| rpcProvider   | the current rpcProvider                                  | string                                      |
| version       | the TrueBlocks version string                            | string                                      |
| chains        | a list of available chains in the config file            | [Chain[]](/data-model/admin/#chain)         |

## Manifest

The Manifest details the portions of the index of appearances which are called ChunkRecords. Each record in the
Manifest details the block range represented by the chunk as well as the IPFS hash of the index chunk along with
the associated IPFS hash for the Bloom filter of the chunk. The manifest itself is also pushed to IPFS and the
IPFS of the hash of the manifest is published periodically to the Unchained Index smart contract.

The following commands produce and manage Manifests:

- [chifra chunks](/chifra/admin/#chifra-chunks)
- [chifra init](/chifra/admin/#chifra-init)
- [chifra scrape](/chifra/admin/#chifra-scrape)

Manifests consist of the following fields:

| Field         | Description                                                           | Type                                            |
| ------------- | --------------------------------------------------------------------- | ----------------------------------------------- |
| version       | the version string hashed into the chunk data                         | string                                          |
| chain         | the chain to which this manifest belongs                              | string                                          |
| specification | IPFS cid of the specification                                         | ipfshash                                        |
| chunks        | a list of the IPFS hashes of all of the chunks in the unchained index | [ChunkRecord[]](/data-model/admin/#chunkrecord) |

## ChunkRecord

The TrueBlocks index scraper periodically creates a chunked portion of the index so that it can
be more easily stored in a content-addresable data store such as IPFS. We call these
periodically-created chunks ChunkRecords. The format of said item is described here. A pinned
chunk is effectively a relational table relating all of the addresses appearing in the chunk
with a list of appearances appearing in the chunk.

The following commands produce and manage ChunkRecords:

- [chifra chunks](/chifra/admin/#chifra-chunks)
- [chifra init](/chifra/admin/#chifra-init)
- [chifra scrape](/chifra/admin/#chifra-scrape)

ChunkRecords consist of the following fields:

| Field     | Description                                       | Type     |
| --------- | ------------------------------------------------- | -------- |
| range     | the block range (inclusive) covered by this chunk | blkrange |
| bloomHash | the IPFS hash of the bloom filter at that range   | ipfshash |
| indexHash | the IPFS hash of the index chunk at that range    | ipfshash |
| bloomSize | the size of the bloom filter in bytes             | int64    |
| indexSize | the size of the index portion in bytes            | int64    |

## ChunkIndex

The `indexchunk` data model represents internal information about each Unchained Index index chunk.
It is used mostly interenally to study the characteristics of the Unchained Index.

The following commands produce and manage ChunkIndexs:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ChunkIndexs consist of the following fields:

| Field        | Description                                                        | Type     |
| ------------ | ------------------------------------------------------------------ | -------- |
| range        | the block range (inclusive) covered by this chunk                  | blkrange |
| magic        | an internal use only magic number to indicate file format          | string   |
| hash         | the hash of the specification under which this chunk was generated | hash     |
| nAddresses   | the number of addresses in this chunk                              | uint64   |
| nAppearances | the number of appearances in this chunk                            | uint64   |
| size         | the size of the chunk in bytes                                     | uint64   |

## ChunkBloom

The `blooms` data model represents the bloom filter files that front the Unchained Index index
portions. The information here is mostly for internal use only as it includes the size and number
of the bloom filters present as well as the number of addresses inserted into the bloom. This
information is used to study the characteristics of the Unchained Index.

The following commands produce and manage ChunkBlooms:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ChunkBlooms consist of the following fields:

| Field     | Description                                                        | Type     |
| --------- | ------------------------------------------------------------------ | -------- |
| range     | the block range (inclusive) covered by this chunk                  | blkrange |
| magic     | an internal use only magic number to indicate file format          | string   |
| hash      | the hash of the specification under which this chunk was generated | hash     |
| nBlooms   | the number of individual bloom filters in this bloom file          | uint64   |
| nInserted | the number of addresses inserted into the bloom file               | uint64   |
| size      | the size on disc in bytes of this bloom file                       | uint64   |
| byteWidth | the width of the bloom filter                                      | uint64   |

## ChunkAddress

The `addresses` data model is produced by `chifra chunks` and represents the records found in the
addresses table of each Unchained Index chunk. The `offset` and `count` fields represent the
location and number of records in the `appearances` table to which the address table is related.

The following commands produce and manage ChunkAddress:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ChunkAddress consist of the following fields:

| Field   | Description                                                               | Type     |
| ------- | ------------------------------------------------------------------------- | -------- |
| address | the address in this record                                                | address  |
| range   | the block range of the chunk from which this address record was taken     | blkrange |
| offset  | the offset into the appearance table of the first record for this address | uint64   |
| count   | the number of records in teh appearance table for this address            | uint64   |

## IpfsPin

`ipfsPin` represents the date, CID and metadata filename of a single IPFS pinned file.

The following commands produce and manage IpfsPins:

- [chifra chunks](/chifra/admin/#chifra-chunks)

IpfsPins consist of the following fields:

| Field      | Description                                                   | Type     |
| ---------- | ------------------------------------------------------------- | -------- |
| cid        | the CID of the file                                           | ipfshash |
| datePinned | the date the CID was first created                            | string   |
| status     | the status of the file (one of [all|pinned|unpinned|pending]) | string   |
| size       | the size of the file in bytes                                 | int64    |
| fileName   | the metadata name of the pinned file                          | string   |

## ChunkStats

The `stats` data model is produced by `chifra chunks` and brings together various statistical
information such as average number of addresses in an Unchained Index chunk among other information.

The following commands produce and manage ChunkStats:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ChunkStats consist of the following fields:

| Field         | Description                                       | Type     |
| ------------- | ------------------------------------------------- | -------- |
| range         | the block range (inclusive) covered by this chunk | blkrange |
| rangeEnd      | the date of the last block in this range          | datetime |
| nAddrs        | the number of addresses in the chunk              | uint64   |
| nApps         | the number of appearances in the chunk            | uint64   |
| nBlocks       | the number of blocks in the chunk                 | uint64   |
| nBlooms       | the number of bloom filters in the chunk's bloom  | uint64   |
| recWid        | the record width of a single bloom filter         | uint64   |
| bloomSz       | the size of the bloom filters on disc in bytes    | uint64   |
| chunkSz       | the size of the chunks on disc in bytes           | uint64   |
| addrsPerBlock | the average number of addresses per block         | float64  |
| appsPerBlock  | the average number of appearances per block       | float64  |
| appsPerAddr   | the average number of appearances per address     | float64  |
| ratio         | the ratio of appearances to addresses             | float64  |

## MonitorClean

MonitorClean is a report on removing duplicates from monitors.

The following commands produce and manage MonitorCleans:

- [chifra monitors](/chifra/accounts/#chifra-monitors)

MonitorCleans consist of the following fields:

| Field    | Description                                                | Type    |
| -------- | ---------------------------------------------------------- | ------- |
| address  | the address being cleaned                                  | address |
| sizeThen | the number of appearances in the monitor prior to cleaning | int64   |
| sizeNow  | the number of appearances in the monitor after cleaning    | int64   |
| dups     | the number of duplicates removed                           | int64   |

## CacheItem

The `cacheItem` data model is used to display various caches displayed from the `chifra config`
tool.

The following commands produce and manage CacheItems:

- [chifra status](/chifra/admin/#chifra-status)

CacheItems consist of the following fields:

| Field       | Description                                         | Type   |
| ----------- | --------------------------------------------------- | ------ |
| type        | the type of the cache                               | string |
| items       | the individual items in the cache (if --verbose)    | any[]  |
| lastCached  | the date of the most recent item added to the cache | string |
| nFiles      | the number of items in the cache                    | uint64 |
| nFolders    | the number of folders holding that many items       | uint64 |
| path        | the path to the top of the given cache              | string |
| sizeInBytes | the size of the cache in bytes                      | int64  |

## ReportCheck

ChunkCheck reports on the results of tests conducted under chifra chunks --check

The following commands produce and manage ReportChecks:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ReportChecks consist of the following fields:

| Field      | Description                                   | Type     |
| ---------- | --------------------------------------------- | -------- |
| result     | the result of the check                       | string   |
| visitedCnt | the number of visited items in the cache      | uint64   |
| checkedCnt | the number of checks                          | uint64   |
| skippedCnt | the number of skipped checks                  | uint64   |
| passedCnt  | the number of passed checks                   | uint64   |
| failedCnt  | the number of failed checks                   | uint64   |
| msgStrings | an array of messages explaining failed checks | string[] |
| reason     | the reason for the test                       | string   |

## ChunkPin

Reports on the result of the command `chifra chunks manifest --pin [--deep]`.

The following commands produce and manage ChunkPins:

- [chifra chunks](/chifra/admin/#chifra-chunks)

ChunkPins consist of the following fields:

| Field         | Description                                             | Type     |
| ------------- | ------------------------------------------------------- | -------- |
| version       | the version string hashed into the chunk data           | string   |
| chain         | the chain to which this manifest belongs                | string   |
| timestampHash | IPFS cid of file containing timestamps                  | ipfshash |
| specHash      | IPFS cid of the specification                           | ipfshash |
| manifestHash  | IPFS cid of file containing CIDs for the various chunks | ipfshash |

## Chain

The `chain` data model represents the configured chain data found in the `trueBlocks.toml`
configuration file.

The following commands produce and manage Chains:

- [chifra status](/chifra/admin/#chifra-status)
- [chifra config](/chifra/admin/#chifra-config)

Chains consist of the following fields:

| Field          | Description                                                      | Type   |
| -------------- | ---------------------------------------------------------------- | ------ |
| chain          | the common name of the chain                                     | string |
| chainId        | the chain id as reported by the RPC                              | uint64 |
| symbol         | the symbol of the base currency on the chain                     | string |
| rpcProvider    | a valid RPC provider for the chain                               | string |
| remoteExplorer | a remote explorer for the chain such as Etherscan                | string |
| localExplorer  | the local explorer for the chain (typically TrueBlocks Explorer) | string |
| ipfsGateway    | an IPFS gateway for pinning the index if enabled                 | string |

## Base types

This documentation mentions the following basic data types.

| Type     | Description                              | Notes       |
| -------- | ---------------------------------------- | ----------- |
| address  | an '0x'-prefixed 20-byte hex string      | lowercase   |
| any      | any cache item found in the binary cache |             |
| blkrange | a pair of nine-digit block numbers       | zero padded |
| bool     | either `true`, `false`, `1`, or `0`      |             |
| datetime | a JSON formatted date                    | as a string |
| float64  | a double precision float                 | 64 bits     |
| hash     | an '0x'-prefixed 32-byte hex string      | lowercase   |
| int64    | a 64-bit signed integer                  |             |
| ipfshash | a multi-hash produced by IPFS            | mixed-case  |
| string   | a normal character string                |             |
| uint64   | a 64-bit unsigned integer                |             |

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
