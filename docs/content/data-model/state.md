---
title: "State"
description: ""
lead: ""
date: 2021-06-30T12:13:24-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu: 
  data:
    parent: collections
weight: 2300
toc: true
---

These commands compare the balance of an address against a particular token or block.

_For information about how to refine balance queries, see
[the corresponding CLI section](/docs/chifra/chainstate/)_

## State

The balance of an address at a given block.

* CLI: [chifra state](/docs/chifra/chainstate/#chifra-state)
* [API](/api#operation/chainstate-state)

### State fields

|Field|Description|Type (format)|
|----|------------|--------------|
|blockNumber|The block where the transaction appears (if nothing specified, the latest block)|integer|
|balance|The balance at this given block|number|
|address|the queried address|string|
|ether|balance in ether|number|

## Tokens

* CLI: [chifra tokens](/docs/chifra/chainstate/#chifra-tokens)
* [API](/api#operation/chainstate-tokens)

### Token fields

|Field|Description|Type (format)|
|----|------------|--------------|
|blockNumber|The block where the transaction appears (if nothing specified, the latest block)|integer|
|holder|Address of token holder|string|
|balance|amount |number (wei)|
|priorBalance| |number (wei)
|symbol|Token symbol|string|
|decimals||number
|is_contract|Whether the account is a smart contract|boolean|
|is_erc20|Whether the account conforms to the [ERC721 non-fungible token standard](https://ethereum.org/en/developers/docs/standards/tokens/erc-721/)|boolean

