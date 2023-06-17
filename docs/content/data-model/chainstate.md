---
title: "Chain state"
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
weight: 1400
toc: true
---

<!-- markdownlint-disable MD033 MD036 MD041 -->
The data structures produced by tools in the Chain State category provide details on the balances
(ERC20 or ETH) of an address against a particular token or block. Additionally, direct access to
a smart contract's state may be queries with the `chirfa state` tool. Data structures in that case
are specific to the particular smart contract.

Each data structure is created by one or more tools which are detailed below.

## EthState

<!-- markdownlint-disable MD033 MD036 MD041 -->
The `state` object displays information about the type of account associated with an address, the
block the address first appeared on the chain, the proxy address if the address is a proxied smart
contract as well as account balance and a few other things.

The following commands produce and manage EthStates:

- [chifra state](/chifra/chainstate/#chifra-state)

EthStates consist of the following fields:

| Field       | Description                                                                      | Type    |
| ----------- | -------------------------------------------------------------------------------- | ------- |
| blockNumber | the block number at which this state was taken                                   | blknum  |
| address     | the address of the state being queried                                           | address |
| proxy       | if this is a proxy, this is the proxied-to address                               | address |
| balance     | the balance at the address at the given block height                             | wei     |
| nonce       | the nonce of the address at the given block height                               | uint64  |
| code        | the byte code at the address (if this is a smart contract)                       | bytes   |
| deployed    | the block number at which this smart contract was deployed (if a smart contract) | blknum  |
| accttype    | the type of the address at the given block                                       | string  |

## CallResult

<!-- markdownlint-disable MD033 MD036 MD041 -->
For the `chifra state --call` tool, the `result` is the result returned by the call to the smart
contract. This is the decoded `output` value of the smart contract call.

The following commands produce and manage CallResults:

- [chifra state](/chifra/chainstate/#chifra-state)

CallResults consist of the following fields:

| Field            | Description                                       | Type    |
| ---------------- | ------------------------------------------------- | ------- |
| blockNumber      | the block number at which this call was made      | blknum  |
| address          | the address of contract being called              | address |
| name             | the name of the function call                     | string  |
| encoding         | the encoding for the function call                | string  |
| signature        | the canonical signature of the interface          | string  |
| encodedArguments | the bytes data following the encoding of the call | string  |
| outputs          | the result of the call to the contract            | Outputs |

## TokenBalance

<!-- markdownlint-disable MD033 MD036 MD041 -->
The data model displays the token balance records for the `chifra tokens` tool.

The following commands produce and manage TokenBalances:

- [chifra tokens](/chifra/chainstate/#chifra-tokens)

TokenBalances consist of the following fields:

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

## EthCall

<!-- markdownlint-disable MD033 MD036 MD041 -->
For the `chifra state --call` tool, the `result` is the result returned by the call to the smart
contract. This is the decoded `output` value of the smart contract call.

The following commands produce and manage EthCalls:

- [chifra state](/chifra/chainstate/#chifra-state)

EthCalls consist of the following fields:

| Field            | Description                                                                      | Type                                    |
| ---------------- | -------------------------------------------------------------------------------- | --------------------------------------- |
| blockNumber      | the block number at which this call was made                                     | blknum                                  |
| address          | the address of contract being called                                             | address                                 |
| signature        | the canonical signature of the interface                                         | string                                  |
| encoding         | the encoding for the function call                                               | string                                  |
| bytes            | the bytes data following the encoding of the call                                | string                                  |
| callResult       | the result of the call to the contract                                           | [Function](/data-model/other/#function) |
| compressedResult | the compressed version of the result of the call to the contract                 | string                                  |
| deployed         | the block number at which this smart contract was deployed (if a smart contract) | blknum                                  |

## Base types

This documentation mentions the following basic data types.

| Type    | Description                         | Notes       |
| ------- | ----------------------------------- | ----------- |
| address | an '0x'-prefixed 20-byte hex string | lowercase   |
| blknum  | an alias for a uint64               |             |
| bool    | either `true`, `false`, `1`, or `0` |             |
| bytes   | an arbitrarily long string of bytes |             |
| string  | a normal character string           |             |
| uint64  | a 64-bit unsigned integer           |             |
| wei     | an unsigned big number              | as a string |
