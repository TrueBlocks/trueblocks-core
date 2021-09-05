---
title: "Accounts"
description: "Explore accounts through associated blockchain data"
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
    parent: collections
weight: 2100
toc: true
---

TrueBlocks Data collections are built progressively.
Each grouping comprises a building block on which another grouping is created.

_These fields describe the output of various Trueblocks account queries.
For information about how to refine these queries, see
[the corresponding CLI section](/docs/chifra/accounts/)_

## Accounts

_Accounts_ link an address to a name.

Accounts are a combination of an`address`, a `name`, and optionally other data. Where possible, this information is queried directly from the blockchain, however in many cases the information was gathered from various websites over the years. For example, every time people say "Show me your address, and we will airdrop some tokens" on Twitter, we copy and paste all those addresses. If you're going to DOX yourselves, we're going to notice. Sorry. Not sorry.

### Account fields

| Field       | Description                                                                     | Type    |
| ----------- | ------------------------------------------------------------------------------- | ------- |
| name        | a user-set text string identifying the address                                  | string  |
| address     | The address of the named account                                                | address |
| tags        | a string catagorizing addresses by type                                         | string  |
| symbol      | ticker-like symbol to identify an account (always acquired on-chain)            | string  |
| source      | text indicating where we found the `name`-`address` association                 | string  |
| is_custom   | `true` for private names not to be shared publically, `false` otherwise         | boolean |
| is_prefund  | `true` if this address was a recipient in the genesis block, `false` otherwise. | boolean |
| is_contract | `true` if this address is a smart contract, `false` otherwise                   | boolean |
| is_erc20    | `true` if this address is an ERC20 token, `false` otherwise                     | boolean |
| is_erc721   | `true` if this address is an ERC721 token, `false` otherwise                    | boolean |

## Monitors actively update collections

Monitors are [Accounts](#account), but they have the special quality that the user has told us explicitly that they are interested in 'monitoring' the activity on the given address. He/she may do that by running either `chifra list` or `chifra export` on the address. Doing so creates a monitor for that address. See the Account data description above for information on the data fields of a monitor.

Monitors can also be [Collections](#collection) of Accounts that are similar to straight up Account monitors but monitor multiple addresses at a time.

### Monitor Fields

See Account fields above.

## Tags

Tags allow you to assign categories to Accounts. Separate tags with a colon (:) when adding new names.

Running `chifra --tags` will list all currently available tags.

## ABIS

For more information on ABIs please see any relevant Ethereum documentation, particularly that documentation related to Solidity coding. The fields or the ABI are mostly identical to the fields you will find in that documentation.

### ABI fields

| Field           | Description                                               | Type         |
| --------------- | --------------------------------------------------------- | ------------ |
| name            | the name of the function or event being described         | string       |
| type            | one of `function` or `event`                              | string       |
| abi_source      | the source of the ABI file (frequently EtherScan)         | string       |
| anonymous       | unused                                                    | boolean      |
| constant        | unused                                                    | boolean      |
| stateMutability | unused                                                    | string       |
| signature       | the Solidity function signature excluding parameter names | string       |
| encoding        | the hash of the signature of the event or function        | hash         |
| inputs          | the input fields to the function or event                 | object array |
| outputs         | the output fields of the function or event                | object array |

° **Input/Output fields**

| Field        | Description                                                                       | Type        |
| ------------ | --------------------------------------------------------------------------------- | ----------- |
| type         | the data type (see Solidity docs) of the parameter                                | string      |
| name         | the name of the parameter                                                         | string      |
| internalType | if the parameter is a struct, this is the object string for the underlying struct | JSON string |

## Function Signatures

## Event Signatures

Both of these data structures are identical to the above ABI fields. When articulating a transaction, event, or trace the ABI file is basically just an array of function and event types.
