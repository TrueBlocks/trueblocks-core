---
title: "Chain state"
description: ""
lead: ""
date: 2022-12-15T07:04:10
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

The data structures produced by tools in the Chain State category provide details on the balances (ERC20 or ETH) of an address against a particular token or block. Additionally, direct access to a smart contract's state may be queries with the `chirfa state` tool. Data structures in that case are specific to the particular smart contract.

_Each data structure is created by one or more tools which are detailed below_

## State

The following commands produce and manage states:

| Tools |     |
| ----- | --- |

The balance of an address at a given block.

* CLI: [chifra state](/docs/chifra/chainstate/#chifra-state)
* [API](/api#operation/chainstate-state)

State data is made of the following data fields:

| Field       | Description                                                                     | Type    |
| ----------- | ------------------------------------------------------------------------------- | ------- |
| blockNumber | the block number at which this state was taken                                  | blknum  |
| address     | the address of the state being queried                                          | address |
| proxy       | if this is a proxy, this is the proxied-to address                              | address |
| balance     | the balance at the address at the given block height                            | wei     |
| nonce       | the nonce of the address at the given block height                              | uint64  |
| code        | the byte code at the address (if this is a smart contract)                      | bytes   |
| deployed    | the block number at which this smart contract was deployed (if a smart contact) | blknum  |
| accttype    | the type of the address at the given block                                      | string  |


## Transfer


| Field            | Description                                                                                    | Type      |
| ---------------- | ---------------------------------------------------------------------------------------------- | --------- |
| blockNumber      | the number of the block                                                                        | blknum    |
| transactionIndex | the zero-indexed position of the transaction in the block                                      | blknum    |
| logIndex         | the zero-indexed position of the log in the transaction                                        | blknum    |
| transactionHash  | the hash of the transaction that triggered this reconciliation                                 | hash      |
| timestamp        | the Unix timestamp of the object                                                               | timestamp |
| date             | a calculated field -- the date of this transaction                                             | datetime  |
| sender           | the initiator of the transfer (the sender)                                                     | address   |
| recipient        | the receiver of the transfer (the recipient)                                                   | address   |
| assetAddr        | 0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee for ETH reconcilations, the token address otherwise | address   |
| assetSymbol      | either ETH, WEI or the symbol of the asset being reconciled as queried from the chain          | string    |
| decimals         | Equivalent to the queried value of `decimals` from an ERC20 contract or, if ETH or WEI then 18 | uint64    |
| amount           | the amount of the transfer in the units of the asset                                           | uint256   |
| spotPrice        | The on-chain price in USD (or if a token in ETH, or zero) at the time of the transaction       | double    |
| priceSource      | The on-chain source from which the spot price was taken                                        | string    |
| encoding         | The four-byte encoding of the transaction's function call                                      | string    |

## Result


| Field            | Description                                                                     | Type      |
| ---------------- | ------------------------------------------------------------------------------- | --------- |
| blockNumber      | the block number at which this call was made                                    | blknum    |
| address          | the address of contract being called                                            | address   |
| signature        | the canonical signature of the interface                                        | string    |
| encoding         | the encoding for the function call                                              | string    |
| bytes            | the bytes data following the encoding of the call                               | string    |
| callResult       | the result of the call to the contract                                          | CFunction |
| compressedResult | the compressed version of the result of the call to the contract                | string    |
| deployed         | the block number at which this smart contract was deployed (if a smart contact) | blknum    |

## Token

The following commands produce and manage tokens:

| Tools |     |
| ----- | --- |

* CLI: [chifra tokens](/docs/chifra/chainstate/#chifra-tokens)
* [API](/api#operation/chainstate-tokens)

Token data is made of the following data fields:

| Field      | Description                                                  | Type    |
| ---------- | ------------------------------------------------------------ | ------- |
| holder     | the address for which we are reporting the token balance     | address |
| balance    | the balance at the address at the given block height         | wei     |
| address    | the address of the token contract being queried              | address |
| name       | the name of the token contract, if available                 | string  |
| symbol     | the symbol for this token contract                           | string  |
| decimals   | the number of decimals for the token contract                | uint64  |
| isContract | `true` if the address is a smart contract, `false` otherwise | bool    |
| isErc20    | `true` if the address is an ERC20, `false` otherwise         | bool    |
| isErc721   | `true` if the address is an ERC720, `false` otherwise        | bool    |


## Base types

This documentation mentions the following basic data types.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| address   | a 20-byte hexadecimal string starting with '0x' | lowercase      |
| blknum    | an alias for a uint64                           |                |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| bytes     | an arbitrarily long string of bytes             |                |
| datetime  | a JSON formatted date                           | as a string    |
| double    | a floating point number of double precision     |                |
| hash      | a 32-byte hexadecimal string starting with '0x' | lowercase      |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | Unix timestamp |
| uint256   |                                                 |                |
| uint64    | a 64-bit unsigned integer                       |                |
| wei       | an unsigned big number                          | as a string    |
