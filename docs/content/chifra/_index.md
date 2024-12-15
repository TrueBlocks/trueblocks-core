---
title : "Chifra Command Line"
description: "The TrueBlocks CLI"
lead: ""
date: 2020-10-06T08:48:45+00:00
aliases:
 - "/docs/chifra/"
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
---

The chifra command provides access to all the applications and tools available:

```[shell]
Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).

  Accounts:
    list          list every appearance of an address anywhere on the chain
    export        export full detail of transactions for one or more addresses
    monitors      add, remove, clean, and list address monitors
    names         query addresses or names of well known accounts
    abis          fetches the ABI for a smart contract
  Chain Data:
    blocks        retrieve one or more blocks from the chain or local cache
    transactions  retrieve one or more transactions from the chain or local cache
    receipts      retrieve receipts for the given transaction(s)
    logs          retrieve logs for the given transaction(s)
    traces        retrieve traces for the given transaction(s)
    when          find block(s) based on date, blockNum, timestamp, or 'special'
  Chain State:
    state         retrieve account balance(s) for one or more addresses at given block(s)
    tokens        retrieve token balance(s) for one or more addresses at given block(s)
  Admin:
    status        report on the status of the TrueBlocks system
    serve         serve the TrueBlocks API using the flame server
    scrape        scan the chain and update (and optionally pin) the TrueBlocks index of appearances
    chunks        manage and investigate chunks and bloom filters
    init          initialize the TrueBlocks system by downloading from IPFS
  Other:
    quotes        update or display Ethereum price data, this tool has been deprecated
    explore       open a local or remote explorer for one or more addresses, blocks, or transactions
    slurp         fetch data from EtherScan for any address
  Flags:
    -h, --help    display this help screen

  Use "chifra [command] --help" for more information about a command.
```

Click on the links to the left for more information on each command
