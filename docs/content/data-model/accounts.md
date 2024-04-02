---
title: "Accounts"
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
weight: 1000
toc: true
---

<!-- markdownlint-disable MD012 MD034 -->
The primary tool of TrueBlocks is `chifra export`. This tool extracts, directly from the chain,
entire transactional histories for one or more addresses and presents that information for use
outside the blockchain. The results of this extraction is stored in a data structure called a
[Monitor](/data-model/accounts/#monitor).

Monitors collect together [Appearances](/data-model/accounts/#appearance) (`blknum.tx_id` pairs)
along with additional information such as [Reconciliations](/data-model/accounts/#reconciliation)
(18-decimal place accurate accounting for each asset transfer), [Names](/data-model/accounts/#names)
(associations of human-readable names with addresses), and [Abis](/data-model/accounts/#abis)
which track the "meaning" of each transaction through its [Functions](/data-model/accounts/#function)
and [Parameters](/data-model/accounts/#parameters).

Each data structure is created by one or more tools which are detailed below.

## Appearance

An appearance is a pointer (`blknum, tx_id` pair) into the blockchain indicating where a
particular address appears. This includes obvious locations such as `to` or `from` as well
as esoteric locations such as deep inside a tenth-level trace or as the miner of an uncle block.
The primary goal of TrueBlocks is to identify every appearance for any address on the chain.

The TrueBlocks [index of appearances](/data-model/the-index/) (created by [chifra scrape](/chifra/admin/#chifra-scrape))
makes the production of such a list possible. Appearances are stored in [Monitors](/data-model/accounts/#monitor).

The following commands produce and manage Appearances:

- [chifra list](/chifra/accounts/#chifra-list)
- [chifra export](/chifra/accounts/#chifra-export)
- [chifra blocks](/chifra/chaindata/#chifra-blocks)
- [chifra chunks](/chifra/admin/#chifra-chunks)

Appearances consist of the following fields:

| Field            | Description                                             | Type      |
| ---------------- | ------------------------------------------------------- | --------- |
| address          | the address of the appearance                           | address   |
| blockNumber      | the number of the block                                 | uint32    |
| transactionIndex | the index of the transaction in the block               | uint32    |
| traceIndex       | the zero-based index of the trace in the transaction    | uint32    |
| reason           | the location in the data where the appearance was found | string    |
| timestamp        | the timestamp for this appearance                       | timestamp |
| date             | the timestamp as a date (calculated)                    | datetime  |

## Monitor

A Monitor is a list of [Appearances](/data-model/accounts/#appearance) associated with a given
address along with various details about those appearances. A monitor is created when a user
expresses interest in an address by calling either [chifra list](/chifra/accounts/#chifra-list)
or [chifra export](/chifra/accounts/#chifra-export) tool (or querying thier associated APIs).

Once created, a monitor may be periodically *freshened* by calling either `chifra list` or `chifra
export`, however, it is also possible to freshen a monitor continually with
[chifra scrape --monitors](/chifra/admin/#chifra-scrape). This tool watches the front of the
chain and repeatedly calls `chifra list`.

The following commands produce and manage Monitors:

- [chifra monitors](/chifra/accounts/#chifra-monitors)
- [chifra list](/chifra/accounts/#chifra-list)
- [chifra export](/chifra/accounts/#chifra-export)

Monitors consist of the following fields:

| Field       | Description                                | Type    |
| ----------- | ------------------------------------------ | ------- |
| address     | the address of this monitor                | address |
| nRecords    | the number of appearances for this monitor | int64   |
| fileSize    | the size of this monitor on disc           | int64   |
| lastScanned | the last scanned block number              | uint32  |
| deleted     | `true` if this monitor has been deleted    | bool    |

## Name

TrueBlocks allows you to associate a human-readable name with an address. This feature goes a long
way towards making the blockchain data one extracts with a [Monitor](/data-model/accounts/#monitor)
much more readable.

Unlike the blockchain data itself, which is globally available and impossible to censor, the
association of names with address is not on chain (excepting ENS, which, while fine, is incomplete).
TrueBlocks allows you to name addresses of interest to you and either share those names (through
an on-chain mechanism) or keep them private if you so desire.

Over the years, we've paid careful attention to the 'airwaves' and have collected together a
'starter-set' of named addresses which is available through the [chifra names](/chifra/accounts/#chifra-names)
command line. For example, every time people say "Show me your address, and we will airdrop some
tokens" on Twitter, we copy and paste all those addresses. We figure if you're going to DOX
yourselves, we might as well take advantage of it. Sorry...not sorry.

The following commands produce and manage Names:

- [chifra names](/chifra/accounts/#chifra-names)

Names consist of the following fields:

| Field      | Description                                                                         | Type    |
| ---------- | ----------------------------------------------------------------------------------- | ------- |
| tags       | colon separated list of tags                                                        | string  |
| address    | the address associated with this name                                               | address |
| name       | the name associated with this address (retrieved from on-chain data if available)   | string  |
| symbol     | the symbol for this address (retrieved from on-chain data if available)             | string  |
| source     | user supplied source of where this name was found (or on-chain if name is on-chain) | string  |
| decimals   | number of decimals retrieved from an ERC20 smart contract, defaults to 18           | uint64  |
| petname    | the petname such as described here http://www.erights.org/elib/capability/pnml.html | string  |
| deleted    | `true` if deleted, `false` otherwise                                                | bool    |
| isCustom   | `true` if the address is a custom address, `false` otherwise                        | bool    |
| isPrefund  | `true` if the address was one of the prefund addresses, `false` otherwise           | bool    |
| isContract | `true` if the address is a smart contract, `false` otherwise                        | bool    |
| isErc20    | `true` if the address is an ERC20, `false` otherwise                                | bool    |
| isErc721   | `true` if the address is an ERC720, `false` otherwise                               | bool    |

## AppearanceCount

The `appearanceCount` data model is used mostly by the frontend explorer application. It carries
various information about the monitor data for an address.

The following commands produce and manage AppearanceCounts:

- [chifra list](/chifra/accounts/#chifra-list)
- [chifra export](/chifra/accounts/#chifra-export)

AppearanceCounts consist of the following fields:

| Field    | Description                                               | Type    |
| -------- | --------------------------------------------------------- | ------- |
| address  | the address for this count                                | address |
| nRecords | the number of appearances for the given address           | uint64  |
| fileSize | the size of the monitor file containing those appearances | uint64  |

## Bounds

The Bounds data model displays information about a given address including how many times it's appeared on the chain and when the first and most recent blocks, timestamps, and dates are.

The following commands produce and manage Bounds:

- [chifra list](/chifra/accounts/#chifra-list)

Bounds consist of the following fields:

| Field      | Description                                                                  | Type          |
| ---------- | ---------------------------------------------------------------------------- | ------------- |
| count      | the number of appearances for this address                                   | uint64        |
| firstApp   | the block number and transaction id of the first appearance of this address  | RawAppearance |
| firstTs    | the timestamp of the first appearance of this address                        | timestamp     |
| firstDate  | the first appearance timestamp as a date (calculated)                        | datetime      |
| latestApp  | the block number and transaction id of the latest appearance of this address | RawAppearance |
| latestTs   | the timestamp of the latest appearance of this address                       | timestamp     |
| latestDate | the latest appearance timestamp as a date (calculated)                       | datetime      |

## Statement

When exported with the `--accounting` option from `chifra export`, each transaction will have field
called `statements`. Statements are an array for reconciliations. All such exported transactions
will have at least one reconciliation (for ETH), however, many will have additional reconciliations
for other assets (such as ERC20 and ERC721 tokens).

Because DeFi is essentially swaps and trades around ERC20s, and because and 'programmable money'
allows for unlimited actions to happen under a single transaction, many times a transaction has
four or five reconciliations.

Reconciliations are relative to an `accountedFor` address. For this reason, the same transaction
will probably have different reconciliations depending on the `accountedFor` address. Consider a
simple transfer of ETH from one address to another. Obviously, the sender's and the recipient's
reconciliations will differ (in opposite proportion to each other). The `accountedFor` address
is always present as the `assetAddress` in the first reconciliation of the statements array.

The following commands produce and manage Statements:

- [chifra export](/chifra/accounts/#chifra-export)

Statements consist of the following fields:

| Field               | Description                                                                                                                           | Type      |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------- | --------- |
| blockNumber         | the number of the block                                                                                                               | blknum    |
| transactionIndex    | the zero-indexed position of the transaction in the block                                                                             | blknum    |
| logIndex            | the zero-indexed position the log in the block, if applicable                                                                         | blknum    |
| transactionHash     | the hash of the transaction that triggered this reconciliation                                                                        | hash      |
| timestamp           | the Unix timestamp of the object                                                                                                      | timestamp |
| date                | the timestamp as a date (calculated)                                                                                                  | datetime  |
| assetAddr           | 0xeeee...eeee for ETH reconciliations, the token address otherwise                                                                    | address   |
| assetSymbol         | either ETH, WEI, or the symbol of the asset being reconciled as extracted from the chain                                              | string    |
| decimals            | the value of `decimals` from an ERC20 contract or, if ETH or WEI, then 18                                                             | uint64    |
| spotPrice           | the on-chain price in USD (or if a token in ETH, or zero) at the time of the transaction                                              | double    |
| priceSource         | the on-chain source from which the spot price was taken                                                                               | string    |
| accountedFor        | the address being accounted for in this reconciliation                                                                                | address   |
| sender              | the initiator of the transfer (the sender)                                                                                            | address   |
| recipient           | the receiver of the transfer (the recipient)                                                                                          | address   |
| begBal              | the beginning balance of the asset prior to the transaction                                                                           | int256    |
| amountNet           | totalIn - totalOut (calculated)                                                                                                       | int256    |
| endBal              | the on-chain balance of the asset (see notes about intra-block reconciliations)                                                       | int256    |
| reconciliationType  | one of `regular`, `prevDiff-same`, `same-nextDiff`, or `same-same`. Appended with `eth` or `token` (calculated)                       | string    |
| reconciled          | true if `endBal === endBalCalc` and `begBal === prevBal`. `false` otherwise. (calculated)                                             | bool      |
| totalIn             | the sum of the following `In` fields (calculated)                                                                                     | int256    |
| amountIn            | the top-level value of the incoming transfer for the accountedFor address                                                             | int256    |
| internalIn          | the internal value of the incoming transfer for the accountedFor address                                                              | int256    |
| selfDestructIn      | the incoming value of a self-destruct if recipient is the accountedFor address                                                        | int256    |
| minerBaseRewardIn   | the base fee reward if the miner is the accountedFor address                                                                          | int256    |
| minerNephewRewardIn | the nephew reward if the miner is the accountedFor address                                                                            | int256    |
| minerTxFeeIn        | the transaction fee reward if the miner is the accountedFor address                                                                   | int256    |
| minerUncleRewardIn  | the uncle reward if the miner who won the uncle block is the accountedFor address                                                     | int256    |
| correctingIn        | for unreconciled token transfers only, the incoming amount needed to correct the transfer so it balances                              | int256    |
| prefundIn           | at block zero (0) only, the amount of genesis income for the accountedFor address                                                     | int256    |
| totalOut            | the sum of the following `Out` fields (calculated)                                                                                    | int256    |
| amountOut           | the amount (in units of the asset) of regular outflow during this transaction                                                         | int256    |
| internalOut         | the value of any internal value transfers out of the accountedFor account                                                             | int256    |
| correctingOut       | for unreconciled token transfers only, the outgoing amount needed to correct the transfer so it balances                              | int256    |
| selfDestructOut     | the value of the self-destructed value out if the accountedFor address was self-destructed                                            | int256    |
| gasOut              | if the transaction's original sender is the accountedFor address, the amount of gas expended                                          | int256    |
| totalOutLessGas     | totalOut - gasOut (calculated)                                                                                                        | int256    |
| prevBal             | the account balance for the given asset for the previous reconciliation                                                               | int256    |
| begBalDiff          | difference between expected beginning balance and balance at last reconciliation, if non-zero, the reconciliation failed (calculated) | int256    |
| endBalDiff          | endBal - endBalCalc, if non-zero, the reconciliation failed (calculated)                                                              | int256    |
| endBalCalc          | begBal + amountNet (calculated)                                                                                                       | int256    |
| correctingReason    | the reason for the correcting entries, if any                                                                                         | string    |

## Base types

This documentation mentions the following basic data types.

| Type      | Description                         | Notes          |
| --------- | ----------------------------------- | -------------- |
| address   | an '0x'-prefixed 20-byte hex string | lowercase      |
| blknum    | an alias for a uint64               |                |
| bool      | either `true`, `false`, `1`, or `0` |                |
| datetime  | a JSON formatted date               | as a string    |
| double    | a double precision float            | 64 bits        |
| hash      | an '0x'-prefixed 32-byte hex string | lowercase      |
| int64     | a 64-bit signed integer             |                |
| int256    | a signed big number                 | as a string    |
| string    | a normal character string           |                |
| timestamp | a 64-bit unsigned integer           | Unix timestamp |
| uint32    | a 32-bit unsigned integer           |                |
| uint64    | a 64-bit unsigned integer           |                |

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
