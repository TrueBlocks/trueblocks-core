---
title: "Chain State"
description: ""
lead: ""
date: 2021-06-30T12:13:03-03:00
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
weight: 1600
toc: true
---

These commands compare the balance of an address against a particular token or block.

_For information about how to refine balance queries, see
[the corresponding CLI section](/docs/chifra/chainstate/)_

## State

The balance of an address at a given block.

* CLI: [chifra state](/docs/chifra/chainstate/#chifra-state)
* [API](/api#operation/chainstate-state)

### Fields

| Field | Description | Type |
|-------|-------------|------|
| blockNumber | the block number at which this state was taken | blknum |
| balance | the balance at the address at the given block height | wei |
| nonce | the nonce of the address at the given block height | uint64 |
| code | the byte code at the address (if this is a smart contract) | bytes |
| storage | this field is un-implemented and current returns the first storage location in a smart contract | bytes |
| address | the address of the state being queried | address |
| deployed | the block number at which this smart contract was deployed (if a smart contact) | blknum |
| accttype | the type of the address at the given block | string |
| ether | the balance of the address in ethers | double |

## Token

* CLI: [chifra tokens](/docs/chifra/chainstate/#chifra-tokens)
* [API](/api#operation/chainstate-tokens)

### Fields

| Field | Description | Type |
|-------|-------------|------|
| holder | the address for which we are reporting the token balance | address |
| balance | the balance at the address at the given block height | wei |
| address | description: the address of the token contract being queried | address |
| name | the name of the token contract&#44; if available | string |
| symbol | the symbol for this token contract | string |
| decimals | the number of decimals for the token contract | uint64 |
| is_contract | `true` if the address is a smart contract&#44; `false` otherwise | bool |
| is_erc20 | `true` if the address is an ERC20&#44; `false` otherwise | bool |
| is_erc721 | `true` if the address is an ERC720&#44; `false` otherwise | bool |
## Base types

In these docs, sometimes Trueblocks mentions a type format that is more
precise than the generic types, like "string" or "object".

| Type Name | Description                         |
| --------- | ----------------------------------- |
| blknum    | a 64-bit unsigned int               |
| timestamp | a 64-bit unsigned int               |
| address   | a 20 byte string starting with '0x' |
| hash      | a 32 byte string starting with '0x' |
| string    | a plain c++ string                  |
| number    | standard c++ 64-bit unsigned int    |
| bigint    | arbitrarily sized signed int        |
| wei       | arbitrarily sized unsigned int      |
| boolean   | standard c++ boolean                |
