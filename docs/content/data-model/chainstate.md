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

## Result

For the `chifra state --call` tool, the `result` is the result returned by the call to the smart
contract. This is the decoded `output` value of the smart contract call.

The following commands produce and manage Results:

- [chifra state](/chifra/chainstate/#chifra-state)

Results consist of the following fields:

| Field            | Description                                               | Type                                    |
| ---------------- | --------------------------------------------------------- | --------------------------------------- |
| blockNumber      | the block number at which this call was made              | blknum                                  |
| timestamp        | the timestamp of the block for this call                  | timestamp                               |
| date             | the date of the block for this call (ulated) (calculated) | datetime                                |
| address          | the address of contract being called                      | address                                 |
| name             | the name of the function call                             | string                                  |
| encoding         | the encoding for the function call                        | string                                  |
| signature        | the canonical signature of the interface                  | string                                  |
| encodedArguments | the bytes data following the encoding of the call         | string                                  |
| articulatedOut   | the result of the call articulated as other models        | [Function](/data-model/other/#function) |

## State

For the `chifra state --call` tool, the `result` is the result returned by the call to the smart
contract. This is the decoded `output` value of the smart contract call.

The following commands produce and manage States:

- [chifra state](/chifra/chainstate/#chifra-state)

States consist of the following fields:

| Field       | Description                                                                   | Type      |
| ----------- | ----------------------------------------------------------------------------- | --------- |
| blockNumber | the block number at which this call was made                                  | blknum    |
| timestamp   | the timestamp of the block for this call                                      | timestamp |
| date        | the date of the block for this call (ulated) (calculated)                     | datetime  |
| address     | the address of contract being called                                          | address   |
| accountType | the type of account at the given block                                        | string    |
| balance     | the balance of the account at the given block                                 | wei       |
| code        | the code of the account                                                       | string    |
| deployed    | for smart contracts only, the block number at which the contract was deployed | blknum    |
| nonce       | the nonce of the account at the given block                                   | uint64    |
| proxy       | the proxy address of the account at the given block                           | address   |

## Token

The `token` data model represents the name, decmials, token symbol, and optionally the totalSupply
of an ERC-20 token.

The following commands produce and manage Tokens:

- [chifra tokens](/chifra/chainstate/#chifra-tokens)
- [chifra export](/chifra/accounts/#chifra-export)

Tokens consist of the following fields:

| Field            | Description                                                           | Type      |
| ---------------- | --------------------------------------------------------------------- | --------- |
| blockNumber      | the block at which the report is made                                 | blknum    |
| transactionIndex | the transaction index (if applicable) at which the report is made     | blknum    |
| timestamp        | the timestamp of the block                                            | timestamp |
| date             | the timestamp as a date (calculated)                                  | datetime  |
| totalSupply      | the total supply of the token contract                                | int256    |
| address          | the address of the token contract                                     | address   |
| holder           | the holder address for which we are reporting                         | address   |
| priorBalance     | the holder's asset balance at its prior appearance                    | int256    |
| balance          | the holder's asset balance at the given block height                  | int256    |
| diff             | the difference#&44; if any#&44; between the prior and current balance | int256    |
| name             | the name of the token contract, if available                          | string    |
| symbol           | the symbol of the token contract                                      | string    |
| decimals         | the number of decimals for the token contract                         | uint64    |
| type             | the type of token (ERC20 or ERC721) or none                           | TokenType |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                         | Notes          |
| --------- | ----------------------------------- | -------------- |
| address   | an '0x'-prefixed 20-byte hex string | lowercase      |
| blknum    | an alias for a uint64               |                |
| datetime  | a JSON formatted date               | as a string    |
| int256    | a signed big number                 | as a string    |
| string    | a normal character string           |                |
| timestamp | a 64-bit unsigned integer           | Unix timestamp |
| uint64    | a 64-bit unsigned integer           |                |
| wei       | an unsigned big number              | as a string    |
