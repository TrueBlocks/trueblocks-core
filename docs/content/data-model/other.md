---
title: "Other"
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
weight: 1800
toc: true
---

These commands call some useful miscellaneous tools:
* `chifra quotes`, a rudimentary pricing tool,
* `chifra explore` a quick way to open a blockchain explorer,
* `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of ceentralization and data quality, see explanation in its section).

{{< alert icon="👉" title="note about keys"
text="Note: some of these tools, like `ethslurp`, require an EtherScan key." >}}
[Follow these instructions to add a key to your config](https://docs.trueblocks.io/docs/prologue/installing-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys)


## Quote

TEXT ABOUT PRICE QUOTES

### How to get price quotes

- **CLI**:
  - run `chifra transactions <txn_id>`.
  - Use the `--articulate` option to turn the `input` field human readable.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-transactions)
- **API**:
  - [Calls to `/transactions`](/api#operation/chaindata-transactions)

### Fields

| Field | Description | Type |
|-------|-------------|------|
| timestamp | the timestamp of this quote | timestamp |
| close | price of the asset in US dollars | double |
| date | the date of the associated timestamp | date |
| name | the name of the block for some named 'special' blocks | string |
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
