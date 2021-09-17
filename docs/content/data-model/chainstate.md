---
title: "Chain State"
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
weight: 1400
toc: true
---

These commands compare the balance of an address against a particular token or block.

_Each data structure is created by one or more tools which are detailed below_

## State

The balance of an address at a given block.

* CLI: [chifra state](/docs/chifra/chainstate/#chifra-state)
* [API](/api#operation/chainstate-state)

### Fields

| Field       | Description                                                                                     | Type    |
| ----------- | ----------------------------------------------------------------------------------------------- | ------- |
| blockNumber | the block number at which this state was taken                                                  | blknum  |
| balance     | the balance at the address at the given block height                                            | wei     |
| nonce       | the nonce of the address at the given block height                                              | uint64  |
| code        | the byte code at the address (if this is a smart contract)                                      | bytes   |
| storage     | this field is un-implemented and current returns the first storage location in a smart contract | bytes   |
| address     | the address of the state being queried                                                          | address |
| deployed    | the block number at which this smart contract was deployed (if a smart contact)                 | blknum  |
| accttype    | the type of the address at the given block                                                      | string  |
| ether       | the balance of the address in ethers                                                            | double  |

## Token

* CLI: [chifra tokens](/docs/chifra/chainstate/#chifra-tokens)
* [API](/api#operation/chainstate-tokens)

### Fields

| Field       | Description                                                  | Type    |
| ----------- | ------------------------------------------------------------ | ------- |
| holder      | the address for which we are reporting the token balance     | address |
| balance     | the balance at the address at the given block height         | wei     |
| address     | description: the address of the token contract being queried | address |
| name        | the name of the token contract, if available                 | string  |
| symbol      | the symbol for this token contract                           | string  |
| decimals    | the number of decimals for the token contract                | uint64  |
| is_contract | `true` if the address is a smart contract, `false` otherwise | bool    |
| is_erc20    | `true` if the address is an ERC20, `false` otherwise         | bool    |
| is_erc721   | `true` if the address is an ERC720, `false` otherwise        | bool    |

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
