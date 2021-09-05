---
title: "Blockchain Data"
description: "TrueBlocks is build on blockchain data. This reference guides you."
lead: "It all starts with transactions and addresses"
date: 2021-07-06T10:43:22-03:00
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
weight: 2050
toc: true
---

_These fields describe the output of various TrueBlocks blockchain queries.
For information about how to refine these queries, see
[the corresponding CLI section](/docs/chifra/chaindata/)_

## Transactions

Transactions represent eth transfers to and from other addresses.

Most of the fields that TrueBlocks returns are standard to all eth transaction.
However, one field is very interesting: `articulatedTx` provides a human readable output of the `input` field.

This is a very powerful way to understand the story behind a smart contract.

### How to get transactions

- **CLI**:
  - run `chifra transactions <txn_id>`.
  - Use the `--articulate` option to turn the `input` field human readable.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-transactions)
- **API**:
  - [Calls to `/transactions`](https://www.tokenomics.io/api.html#/ChainData/chaindata-transactions)
- **Explorer**

### Reference of transaction fields

| Field             | Description                                                                                                    | Type         |
| ----------------- | -------------------------------------------------------------------------------------------------------------- | ------------ |
| hash              | the transaction's hash                                                                                         | hash         |
| blockHash         | the hash of the block in which this transaction appears                                                        | hash         |
| blockNumber       | the block number of the containing block                                                                       | blknum       |
| transactionIndex  | the transaction's zero-indexed position in the block                                                           | blknum       |
| nonce             | sequence number of the transactions sent by the sender                                                         | number       |
| timestamp         | the block's unix timestamp                                                                                     | timestamp    |
| from              | the address from which this transaction was sent                                                               | address      |
| to                | the recipient address for this transaction                                                                     | address      |
| value             | the amount of wei sent with this transactions                                                                  | wei          |
| gas               | the amount of gas the sender is willing to spend on this transaction                                          | wei          |
| gasPrice          | the number of wei per unit of gas used the sender is willing to spend                                          | wei          |
| input             | byte data either containing a message or funcational data for a smart contracts. See the `--articulate` option | bytes        |
|                   |                                                                                                                |              |
| receipt           | [receipt](#receipt) for the transaction                                                                        |
| [traces](#traces) | an array of trace objects                                                                                      | object array |
| statements        | an array of [reconciliations](#reconciliations). One for each asset present in the transaction                 | object array |
| articulatedTx     | a human readable articulation of the `input` field                                                             | object       |
| compressedTx      | truncated, more readable version of the articulation                                                           | string       |
| isError           | `true` if transaction ended in error                                                                           | boolean      |
| hasToken          | `true` if transaction is either ERC20- or ERC721-related                                                       | boolean      |
| finalized         | `true` if TrueBlocks considers this block final                                                                | boolean      |

## Blocks

`chifra blocks` returns top level data specified block. You can also include
an array for the blocks' transactions.

### How to get blocks

- **CLI**:
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-blocks)
- **API**:
  - [Calls to `/transactions`](https://www.tokenomics.io/api.html#/ChainData/chaindata-blocks)
- **Explorer**

### Reference of block fields

| Field           | Description                                                                   | Type         |
| --------------- | ----------------------------------------------------------------------------- | ------------ |
| gasLimit        | the system-wide maximum amount of gas permitted in this block                 | wei          |
| hash            | the hash of the current block                                                 | hash         |
| blockNumber     | the block number of the block                                                 | blknum       |
| parentHash      | the hash of the previous block                                                | hash         |
| miner           | address of the winning miner for this block                                   | address      |
| difficulty      | the computational difficulty at this block                                    | bigint       |
| timestamp       | the unix timestamp of the block                                               | timestamp    |
| transactions    | a possibly empty array of [transactions](#transactions) or transaction hashes | object array |
|                 |                                                                               |              |
| gasUsed         | The amount of gas used                                                        | wei          |
| transactionsCnt | the number of transactions in the array                                       | number       |
| finalized       | a flag indicating if TrueBlocks considers this block final                    | boolean      |

## Receipts

Receipts record the amount of gas used for a transaction among other things. If the transaction succeeded, a receipt might also have logs.

If the `to` address of a transaction is `0x0`, the `input` data is considered to be the the source code (byte code) of a smart contract. In this case, if the creation of the contract succeeds, the `contractAddress` field of the receipt carries the address of the newly created contract.

### How to get receipts

- **CLI**:
  - run `chifra receipts <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-receipts)
- **API**:
  - [Calls to `/receipts`](https://www.tokenomics.io/api.html#/ChainData/chaindata-receipts)
- **Explorer**

### Reference of receipt fields

| Field           | Description                                       | Type         |
| --------------- | --------------------------------------------------| ------------ |
| status          | `1` if the transaction suceeds, `0` otherwise     | number      |
| contractAddress | the address of the newly created contract, if any | address      |
| gasUsed         | the amount of gas used by this transaction        | wei          |
| logs            | a possibly empty array of [logs](#logs)           | object array |

## Logs

Logs appear in a possibly empty array in the transaction's receipt. They are only created if the underlying transaction suceeded. In the case where the transaction failed, no logs will appear in the receipt. Logs are only ever generated during transactions whose `to` address is a smart contract.

### How to get logs

- **CLI**:
  - run `chifra logs <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-logs)
- **API**:
  - [Calls to `/logs`](https://www.tokenomics.io/api.html#/ChainData/chaindata-logs)
- **Explorer**

### Reference of log fields

| Field            | description                                                                                       | type               |
| ---------------- | ------------------------------------------------------------------------------------------------- | ------------------ |
| blockNumber      | the block number of the containing block                                                          | blknum             |
| transactionIndex | the transaction's zero-indexed position in the block                                              | blknum             |
| address          | the smart contract that emitted this log                                                          | address            |
| logIndex         | the zero-indexed position of this log relative to the block                                       | number             |
| topics           | the first topic hashes event signature of the log, up to 3 additional index parameters may appear | hash array (max 4) |
| data             | any remaining un-indexed parameters to the event                                                  | bytes              |
| articulatedLog   | a human-readable version of the `topic` and `data` fields                                         | object             |
| compressedLog    | a truncated, more readable version of the articulation                                            | string             |

<!---emitter is used in chifra export --->

## Traces

The deepest layer of the Ethereum data is the trace. Every transaction has at least one trace which is itself a record of the transaction. If the `to` address of the transaction is a smart contract, other traces may appear, if, for example, that smart contract calls other smart contracts.

Traces may be arbitrarily deep (up to the gasLimit) and ultimately represent a tree of function calls. Some transactions have 100s of traces. The format of the trace is similar to the transaction itself have a trace `action` (which contains `from`, `to`, `value` like the transaction) and the trace `result` (containing `gasUsed` like the receipt).

### How to get traces

- **CLI**:
  - run `chifra traces <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-traces)
- **API**:
  - [Calls to `/traces`](https://www.tokenomics.io/api.html#/ChainData/chaindata-traces)
- **Explorer**

### Reference of trace fields

| Field            | description                                             | type   |
| ---------------- | ------------------------------------------------------- | ------ |
| blockHash        | the hash of the block in which this transaction appears | hash   |
| blockNumber      | the block number of the containing block                | blknum |
| transactionHash  | the transaction's hash                                  | hash   |
| transactionIndex | the transaction's zero-indexed position in the block    | blknum |
| traceAddress     | a particular trace's address in the trace tree          | string |
| subTraces        | the number of children traces that the trace has        | number |
| type             | the type of the trace                                   | string |
| action           | the [trace action](#trace-action)                       | object |
| result           | the [trace result](#trace-result)                       | object |

#### Trace action

Other than the first trace which is the trace of the transaction itself, traces represent calls into smart contracts. Because of this, `trace actions` closely resemble the fields of the [transaction](#transactions).

| Field            | Description                                                                        | Type    |
| ---------------- | ---------------------------------------------------------------------------------- | ------- |
| from             | the smart contract address that called the trace                                   | address |
| to               | the smart contract address that the trace calls                                    | address |
| gas              | The maximum amount of gas allowed for this call                                    | wei     |
| input            | an encoded version of the function call. Use the `--articulate` to display details | bytes   |
| callType         | the type of this call                                                              | string  |
| refundAddress    | if the call type is `self-destruct`, the address to which the refund is sent       | address |
|                  |                                                                                    |         |
| articulatedTrace | A human-readable version of the trace action's `input` and the result's `output`   | object  |
| compressedTrace  | a truncated, more readable version of the articulation                             | string  |

#### Trace result

As mentioned above, other than the first trace, traces represent calls into other smart contracts. Because of this, the trace results closely resembles the fields of the [receipt](#receipts).

| Field       | description                                                                            | type   |
| ----------- | -------------------------------------------------------------------------------------- | ------ |
| newContract | address of the newly created contract, if any                                          | string |
| code        | if this trace is creating a new smart contract, this is the byte code of that contract | bytes  |
| gasUsed     | the amount of gas used during the call                                                 | wei    |
| output      | the result of the call for this trace                                                  | bytes  |

## when block

### How to get when blocks

- **CLI**:
  - run `chifra when <block_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-when)
- **API**:
  - [Calls to `/when`](https://www.tokenomics.io/api.html#/ChainData/chaindata-when)
- **Explorer**

### Reference of whenblock fields

When a block appeared, in unix and human readable format:

| Field       | Description                              | Type      |
| ----------- | ---------------------------------------- | --------- |
| blockNumber | the block number of the containing block | blknum    |
| timestamp   | the block's unix timestamp               | timestamp |
| date        | human readable version of the timestamp  | date      |

## Reconciliations

When exported with the `--accounting` option from `chifra export`, each transaction will have field called `statements`. Statements are an array for reconciliations. All such exported transactions will have at least one reconcilation (for ETH), however, many will have additional reconciliations for other assets (such as ERC20 and ERC721 tokens).

Because DeFi is essentially swaps and trades around ERC20s, and because and 'programmable money' allows for unlimited actions to happen under a single transaction,many times a transaction has four or five reconciliations.

Reconciliations are relative to an `accountedFor` address. For this reason, the same transaction will probably have different reconciliations depending on the `accountedFor` address. Consider a simple transfer of ETH from one address to another. Obviously, the sender's and the recipient's reconciliations will differ (in opposite proportion to each other). The `accountedFor` address is always present as the `assetAddress` in the first reconciliation of the statements array.

**Note on intra-block transactions**: In many cases two or more transactions requiring a reconciliation may occur in a single block. Becuase the Ethereum blockchain only provides balance queries at the end of blocks, it is not possible to query for the balance of an asset at the end of transactions for which there are other following transactions in the block nor for the beginning balance for which there are transactions prior to the given transaction in the same block. In these cases, TrueBlocks simulates the beginning and ending balance as needed and adds `partial` to the `reconciliationType`.

**Note on spotPrice**: If the `spotPrice` is available from an on-chain source (such as UniSwap), then it represents the ETH/DAI value at the time of the transaction if the reconcilation is for ETH. For other assets, the `spotPrice` represents the asset's value relative to `ETH`, so to price a non-ETH asset in US dollars, one would need to convert first to `ETH` then to dollars. If a price is not available on-chain, the `spotPrice` will be zero and the caller is encouraged to get the price for the asset from other sources.

### Reference of reconciliation fields

| Name                | Description                                                                                                                                                                        | Type      |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------- |
| blockNumber         | the block number of the containing block                                                                                                                                           | blknum    |
| transactionIndex    | the transaction's zero-indexed position in the block                                                                                                                               | blknum    |
| timestamp           | the block's unix timestamp                                                                                                                                                         | timestamp |
| assetAddr           | for reconciliations whose `assetSymbol` is `ETH` or `WEI`, the accountedFor address. For all assets, the address of the asset itself                                               | address   |
| assetSymbol         | either `ETH`, `WEI` or the symbol of the asset being reconciled as extracted from the chain                                                                                        | string    |
| decimals            | eEquivilent to the extracted value of getSymbol from ERC20 or, if `ETH` or `WEI` then `18`                                                                                         | number    |
| prevBlk             | the block number of the previous reconciliation                                                                                                                                    | blknum    |
| prevBlkBal          | the account balance for the given asset for the previous reconciliation                                                                                                            | bigint    |
| begBal              | the begining balance of the asset at the `blockNumber`                                                                                                                             | bigint    |
| begBalDiff          | the difference between the expected beginning balance (prevBlkBal) and the queried balance from the chain                                                                          | bigint    |
| amountIn            | for `ETH` reconciliations, the `value` of the transactions if the `from` address is the same as the `accountedFor` address. Otherwise, the value of the ERC20's transfer `amount`. | bigint    |
| amountOut           | the amount (in terms of the asset) of regular outflow during this tx                                                                                                               | bigint    |
| internalIn          | for `ETH` reconciliations only, the value of any internal value transfers into the `accountedFor` account                                                                          | bigint    |
| internalOut         | for `ETH` reconciliations only, the value of any internal value transfers out of the `accountedFor` account                                                                        | bigint    |
| selfDestructIn      | for `ETH` reconciliations for transactions ending in self-destrution only, the value received by the `accountedFor` account from the self-destructed account                       | bigint    |
| selfDestructOut     | for `ETH` reconciliations for transactions ending in self-destrution only, the value transfered out of the `accountedFor` account                                                  | bigint    |
| minerBaseRewardIn   | for blocks won by the `accountedFor` address, this is the base fee reward for the miner                                                                                            | bigint    |
| minerNephewRewardIn | for blocks won by the `accountedFor` address, this is the netphew reward for the miner                                                                                             | bigint    |
| minerTxFeeIn        | for blocks won by the `accountedFor` address, this is the transaction fee reward for the miner                                                                                     | bigint    |
| minerUncleRewardIn  | for blocks in which the `accountedFor` address generated an uncle, this value is the uncle reward                                                                                  | bigint    |
| prefundIn           | at block zero (0) only, the amount of genesis income for the `accountedFor` address                                                                                                | bigint    |
| gasCostOut          | if `accountedFor` address is the transaction's sender (i.e. `from` at the top level), the amount of gas expended denominated in either `ETH` or `WEI`.                             | bigint    |
| endBal              | the balance of the reconciled asset at the end of this transaction found by querying the chain (see the note above about intra-block reconciliations)                              | bigint    |
| totalIn             | a calculated field: the sum of all `In` fields                                                                                                                                     | bigint    |
| totalOut            | a calculated field: the sum of all `Out` fields                                                                                                                                    | bigint    |
| amountNet           | a calculated field: `totalIn` - `totalOut`                                                                                                                                         | bigint    |
| endBalCalc          | a calculated field: `begBal` + `amountNet`                                                                                                                                         | bigint    |
| endBalDiff          | a calculated field: `endBal` - `endBalCalc`, if non-zero, the reconciliation failed                                                                                                | bigint    |
| reconciled          | a calculated field: `true` if `endBal` === `endBalCalc` and `begBal` === `prevBlkBal`. `false` otherwise.                                                                          | bool      |
| reconciliationType  | One of `regular`, `traces`, `prevdiff-partial`, `partial-nextdiff`, or `partial-partial depending on previous, following or both transactions in the same block                    | string    |
| spotPrice           | The price (if available) at the time of the transaction in US dollars for ETH reconciliations and ETH for other assets                                                             | bigint    |

## Reference of type formats

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
