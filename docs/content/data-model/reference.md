---
title: "Enhanced Ethereum data"
description: "A description of the data fields of TrueBlocks"
lead: "The following tables provide references to data you get from TrueBlocks queries."
date: 2021-07-26T12:13:03-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: true
images: []
menu:
  data:
    parent: collections
weight: 2000
toc: true
---

On its own, Ethereum blockchain data is just a boring blob of bytes.
What really interests us is the stories behind this data--questions like:

- When did the transaction happen?
- What addresses were involved?
- Can I see all transactions involving this address?

## Blockchain + people = TrueBlocks-enhanced data

Broadly, the data TrueBlocks provides divides into two categories

- [Blockchain data](../blockchain-data/)
- [Account-organized data](../accounts)

The account-organized data describes things like account collections, account
balances, and transaction histories. This data tells you the _who_, _where_, and
_why_ behind the Blockchain transactions. It is where TrueBlocks really shines.

### This data applies to all TrueBlocks apps

Because all TrueBlocks apps use the same backend, these references describe
the data found with the command line, the Explorer app, and the API. However,
the presentation of the data may change depending on the app and file format.
