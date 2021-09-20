---
title: "Accounts"
description: ""
lead: ""
date: 2021-09-18T14:53:50
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
weight: 1000
toc: true
---

The primary purpose of TrueBlocks is to extract, directly from the blockchain, the entire transactional history for one or more addresses and present that information for use outside the blockchain. The results of this extraction are stored in a data structure called a [Monitor](/data-model/accounts/#monitor).

Monitors collect together [Appearances](/data-model/accounts/#appearance) (`blknum.tx_id` pairs) along with additional information such as [Reconciliations](/data-model/accounts/#reconciliation) (18-decimal place accurate accounting for each asset transfer), [Names](/data-model/accounts/#names) (associations of human-readable names with addresses), and [Abis](/data-model/accounts/#abis) which track the "meaning" of each transaction through its [Functions](/data-model/accounts/#function) and [Parameters](/data-model/accounts/#parameters).

_Each data structure is created by one or more tools which are detailed below_

## Monitor

A Monitor is a list of [Appearances](/data-model/accounts/#appearance) associated with a given address along with various details about those appearances. A monitor is created when a user expresses interest in an address by calling either [chifra list](/docs/chifra/accounts/#chifra-list) or [chifra export](/docs/chifra/accounts/#chifra-export) tool (or querying thier associated APIs).

Once created, a monitor may be periodically *freshened* by calling either `chifra list` or `chifra export`, however, it is also possible to freshen a monitor continually with [chifra scrape --monitors](/docs/chifra/admin/#chifra-scrape). This tool watches the front of the chain and repeatedly calls `chifra list`.

Below is a list of the data fields for monitors. Following that are the commands that produce or manage monitors.

| Field       | Description                                    | Type    |
| ----------- | ---------------------------------------------- | ------- |
| nApps       | the number of appearances for this monitor     | blknum  |
| firstApp    | the first block at which this address appears  | blknum  |
| latestApp   | the latest block at which this address appears | blknum  |
| sizeInBytes | the size of this monitor on disc               | uint64  |
| tags        | the tag given to this address                  | string  |
| address     | the address being monitored                    | address |
| is_custom   | `true` if this address is customized           | bool    |

| Tools                                                       |                                                              |
| ----------------------------------------------------------- | ------------------------------------------------------------ |
| [chifra status monitors](/docs/chifra/admin/#chifra-status) | report on the status of the TrueBlocks system                |
| [chifra list](/docs/chifra/accounts/#chifra-list)           | list appearances for one or more addresses                   |
| [chifra export](/docs/chifra/accounts/#chifra-export)       | export full detail of transactions for one or more addresses |


---

## Appearance

An appearance is a pointer (`blknum, tx_id` pair) into the blockchain indicating where a particular address appears. This includes obvioius locations such as `to` or `from` as well as esoteric locations such as deep inside a tenth-level trace or as the miner of an uncle block. The primary goal of TrueBlocks is to indentify every appearance for any address on the chain.

The TrueBlocks [index of appearances](/data-model/the-index/) (created by [chifra scrape](/docs/chifra/admin/#chifra-scrape)) makes the production of such a list possible. Appearances are stored in [Monitors](http://localhost:1313/data-model/accounts/#monitor).

Below is a list of the data fields for appearances. Following that are the commands that produce or manage appearances.

| Field            | Description                                               | Type      |
| ---------------- | --------------------------------------------------------- | --------- |
| blockNumber      | the number of the block                                   | blknum    |
| transactionIndex | the zero-indexed position of the transaction in the block | blknum    |
| address          | the address of the appearance                             | address   |
| name             | the name of the address, if found                         | string    |
| timestamp        | the timestamp for this appearance                         | timestamp |
| date             | the date represented by the timestamp                     | string    |

| Tools                                                     |                                                              |
| --------------------------------------------------------- | ------------------------------------------------------------ |
| [chifra list](/docs/chifra/accounts/#chifra-list)         | list appearances for one or more addresses                   |
| [chifra export](/docs/chifra/accounts/#chifra-export)     | export full detail of transactions for one or more addresses |
| [chifra monitors](/docs/chifra/accounts/#chifra-monitors) | delete, undelete, and remove previously created monitors     |

---

## Reconciliation

When exported with the `--accounting` option from `chifra export`, each transaction will have field called `statements`. Statements are an array for reconciliations. All such exported transactions will have at least one reconcilation (for ETH), however, many will have additional reconciliations for other assets (such as ERC20 and ERC721 tokens).

Because DeFi is essentially swaps and trades around ERC20s, and because and 'programmable money' allows for unlimited actions to happen under a single transaction,many times a transaction has four or five reconciliations.

Reconciliations are relative to an `accountedFor` address. For this reason, the same transaction will probably have different reconciliations depending on the `accountedFor` address. Consider a simple transfer of ETH from one address to another. Obviously, the sender's and the recipient's reconciliations will differ (in opposite proportion to each other). The `accountedFor` address is always present as the `assetAddress` in the first reconciliation of the statements array.

Below is a list of the data fields for reconciliations. Following that are the commands that produce or manage reconciliations.

| Field               | Description                                                                                                     | Type      |
| ------------------- | --------------------------------------------------------------------------------------------------------------- | --------- |
| blockNumber         | the number of the block                                                                                         | blknum    |
| transactionIndex    | the zero-indexed position of the transaction in the block                                                       | blknum    |
| timestamp           | the unix timestamp of the object                                                                                | timestamp |
| assetAddr           | the accountedFor address for ETH recons, the token address itself otherwise                                     | address   |
| assetSymbol         | either ETH, WEI or the symbol of the asset being reconciled as extracted from the chain                         | string    |
| decimals            | Equivilent to the extracted value of getSymbol from ERC20 or, if ETH or WEI then 18                             | uint64    |
| prevBlk             | the block number of the previous reconciliation                                                                 | blknum    |
| prevBlkBal          | the account balance for the given asset for the previous reconciliation                                         | int256    |
| begBal              | the begining balance of the asset at the blockNumber                                                            | int256    |
| begBalDiff          | the difference between the expected beginning balance (prevBlkBal) and the queried balance from the chain       | int256    |
| amountIn            | the top-level value of the incoming transfer for the accountedFor address                                       | int256    |
| amountOut           | the amount (in terms of the asset) of regular outflow during this bigint                                        | int256    |
| internalIn          | the internal value of the incoming transfer for the accountedFor address                                        | int256    |
| internalOut         | the value of any internal value transfers out of the accountedFor account                                       | int256    |
| selfDestructIn      | the incoming value of a self-destruct if recipient is the accountedFor address                                  | int256    |
| selfDestructOut     | the value of the self-destructed value out if the accountedFor address was self-destructed                      | int256    |
| minerBaseRewardIn   | the base fee reward if the miner is the accountedFor address                                                    | int256    |
| minerNephewRewardIn | the netphew reward if the miner is the accountedFor address                                                     | int256    |
| minerTxFeeIn        | the transaction fee reward if the miner is the accountedFor address                                             | int256    |
| minerUncleRewardIn  | the uncle reward if the miner who won the uncle block is the accountedFor address                               | int256    |
| prefundIn           | at block zero (0) only, the amount of genesis income for the accountedFor address                               | int256    |
| spotPrice           | The on-chain price in USD (or if a token in ETH, or zero) at the time of the transaction                        | double    |
| priceSource         | The on-chain source from which the spot price was taken                                                         | string    |
| gasCostOut          | if the transactions original sender is the accountedFor address, the amount of gas expended denominated in WEI. | int256    |
| endBal              | the on-chain balance of the asset (see notes above about intra-block reconciliations)                           | int256    |
| totalIn             | a calculated field -- the sum of all In fields                                                                  | int256    |
| totalOut            | a calculated field -- the sum of all Out fields                                                                 | int256    |
| totalOutLessGas     |                                                                                                                 | int256    |
| amountNet           | a calculated field -- totalIn - totalOut                                                                        | int256    |
| endBalCalc          | a calculated field -- begBal + amountNet                                                                        | int256    |
| reconciliationType  | One of regular, traces, prevdiff-partial, partial-nextdiff, or `partial-partial`                                | string    |
| endBalDiff          | a calculated field -- endBal - endBalCalc, if non-zero, the reconciliation failed                               | int256    |
| reconciled          | a calculated field -- true if `endBal === endBalCalc` and `begBal === prevBlkBal`. `false` otherwise.           | bool      |

**Notes**

**Intra-block transactions**: In many cases two or more transactions requiring a reconciliation may occur in a single block. Becuase the Ethereum blockchain only provides balance queries at the end of blocks, it is not possible to query for the balance of an asset at the end of transactions for which there are other following transactions in the block nor for the beginning balance for which there are transactions prior to the given transaction in the same block. In these cases, TrueBlocks simulates the beginning and ending balance as needed and adds `partial` to the `reconciliationType`.

**Spot Price**: If the `spotPrice` is available from an on-chain source (such as UniSwap), then it represents the ETH/DAI value at the time of the transaction if the reconcilation is for ETH. For other assets, the `spotPrice` represents the asset's value relative to `ETH`, so to price a non-ETH asset in US dollars, one would need to convert first to `ETH` then to dollars. If a price is not available on-chain, the `spotPrice` will be zero and the caller is encouraged to get the price for the asset from other sources.


---

## Name

TrueBlocks allows you to associate a human-readable name with an address. This feature goes a long way towards making the blockchain data one extracts with a [Monitor](/data-model/accounts/#monitor) much more readable.

Unlike the blockchain data itself, which is globally available and impossible to censor, the association of names with address is not on chain (excepting ENS, which, while fine, is imcomplete). TrueBlocks allows you to name addresses of interest to you and either share those names (through an on-chain mechanism) or keep them private if you so desire.

Over the years, we've paid careful attention to the 'airwaves' and have collected together a 'starter-set' of named addresses which is available through the [chifra names](/docs/chifra/accounts/#chifra-names) command line. For example, every time people say "Show me your address, and we will airdrop some tokens" on Twitter, we copy and paste all those addresses. We figure if you're going to DOX yourselves, we might as well take advantage of it. Sorry...not sorry.

Below is a list of the data fields for names. Following that are the commands that produce or manage names.

| Field       | Description                                                                         | Type    |
| ----------- | ----------------------------------------------------------------------------------- | ------- |
| tags        | colon separated list of tags                                                        | string  |
| address     | the address associated with this name                                               | address |
| name        | the name associated with this address (retreived from on-chain data if available)   | string  |
| symbol      | the symbol for this address (retreived from on-chain data if available)             | string  |
| source      | user supplied source of where this name was found (or on-chain if name is on-chain) | string  |
| decimals    | number of decimals retreived from an ERC20 smart contract, defaults to 18           | uint64  |
| description | user supplied description for the address                                           | string  |
| deleted     | `true` if deleted, `false` otherwise                                                | bool    |
| is_custom   | `true` if the address is a custom address, `false` otherwise                        | bool    |
| is_prefund  | `true` if the address was one of the prefund addresses, `false` otherwise           | bool    |
| is_contract | `true` if the address is a smart contract, `false` otherwise                        | bool    |
| is_erc20    | `true` if the address is an ERC20, `false` otherwise                                | bool    |
| is_erc721   | `true` if the address is an ERC720, `false` otherwise                               | bool    |

| Tools                                               |                                                 |
| --------------------------------------------------- | ----------------------------------------------- |
| [chifra names](/docs/chifra/accounts/#chifra-names) | query addresses or names of well known accounts |


---

## Abi

An ABI describes an Application Binary Interface -- in other words, the [Function]() and Event signatures for a given smart contract. Along with [Names]() the use of ABIs goes a very long way towards making your Etheruem data much more understandable.

Similar to names of addresses, ABI files are not available on-chain which means they must be acquired somewhere. Unfortantely, the Ethereum community has not yet understand that EtherScan is not a good place to store this very important information. For this reason, TrueBlocks uses EtherScan to acquire ABI files and therefor one needs to get an EtherScan API key to use this function.

Below is a list of the data fields for abis. Following that are the commands that produce or manage abis.

| Field      | Description                                  | Type           |
| ---------- | -------------------------------------------- | -------------- |
| address    | the smart contract that implements this abi  | address        |
| interfaces | the list of events and functions on this abi | CFunctionArray |

| **Tools**                                         |                                      |
| ------------------------------------------------- | ------------------------------------ |
| [chifra abis](/docs/chifra/accounts/#chifra-abis) | fetches the ABI for a smart contract |

**Notes**

See the `chifra abis` command line for information about getting an EtherScan key.

---

## Function

ABI files are derived from the Solidity source code of a smart contract by extracting the canonical function and event signatures in a JSON structure. The function signatures are hashed (using keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks commands provide an `--articulate` option. See the commands themselves for more information.

Below is a list of the data fields for functions. Following that are the commands that produce or manage functions.

| Field     | Description                                             | Type            |
| --------- | ------------------------------------------------------- | --------------- |
| name      | the name of the interface                               | string          |
| type      | the type of the interface, either 'event' or 'function' | string          |
| signature | the canonical signature of the interface                | string          |
| encoding  | the signature encoded with keccak                       | string          |
| inputs    | the input parameters to the function, if any            | CParameterArray |
| outputs   | the output parameters to the function, if any           | CParameterArray |

| Tools                                                 |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chaindata commands](/docs/chifra/chaindata/)         | various commands dealing with blockchain data                |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |


---

## Parameter

Parameters are a constituant part of a [Function or Event](/data-model/accounts/#function). The parameters of a function are each individual value passed into the function. Along with the function's name, the parameters types (once canonicalized) are used to create a function's four byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because we use them as part of the ABI decoding and the `--articulate` process to conver the blockchain's bytes into human-readable text.

Below is a list of the data fields for parameters. Following that are the commands that produce or manage parameters.

| Field        | Description                                                 | Type            |
| ------------ | ----------------------------------------------------------- | --------------- |
| type         | the type of this parameter                                  | string          |
| name         | the name of this parameter                                  | string          |
| str_default  | the defaul value of this parameter, if any                  | string          |
| indexed      | `true` if this parameter is indexed                         | bool            |
| internalType | for composite types, the interal type of the parameter      | string          |
| components   | for composite types, the parameters making up the composite | CParameterArray |

| Tools                                             |                                      |
| ------------------------------------------------- | ------------------------------------ |
| [chifra abis](/docs/chifra/accounts/#chifra-abis) | fetches the ABI for a smart contract |


---

## Base types

The above documentation mentions the following basic data types.

| Type      | Description                                     | Notes          |
| --------- | ----------------------------------------------- | -------------- |
| address   | a 20-byte hexidecimal string starting with '0x' | lowercase      |
| blknum    | an alias for a uint64                           |                |
| bool      | a value either `true`, `false`, `1`, or `0`     |                |
| double    | a floating point number of double precision     |                |
| int256    | a signed big number                             | as a string    |
| string    | a normal character string                       |                |
| timestamp | a 64-bit unsigned integer                       | unix timestamp |
| uint64    | a 64-bit unsigned integer                       |                |
