---
title: "Globals"
description: ""
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
aliases:
 - "/docs/chifra/accounts"
menu:
  chifra:
    parent: commands
weight: 2200
toc: true
---
## Global Options

All commands have these globally available options unless removed as shown in the Disabled column below. Some commands have additional globally available features as noted in the Enabled column.

| Option                                                                      |
| --------------------------------------------------------------------------- |
| Verbose, Fmt, Version, Noop, NoColor, Chain, NoHeader, File, Output, Append |

### Group 1

These commands produce data but do not need to use the cache because they do not query the node.

| Group    | Cmd      | Enabled | Disabled |
| -------- | -------- | ------- | -------- |
| accounts | list     |         |          |
| accounts | monitors |         |          |
| accounts | names    |         |          |
| admin    | config   |         |          |
| admin    | status   |         |          |
| admin    | chunks   |         |          |

### Group 2

These commands produce data, but because they do query the node, the need the cache.

| Group       | Cmd    | Enabled | Disabled |
| ----------- | ------ | ------- | -------- |
| accounts    | abis   | Caching |          |
| chain_data  | when   | Caching |          |
| chain_state | tokens | Caching |          |

### Group 3

These commands produce data, but because they do query the node, the need the cache. They also have wei values which are allowed to use `--ether` option to convert to ether. The --raw option is not available.

| Group       | Cmd    | Enabled             | Disabled |
| ----------- | ------ | ------------------- | -------- |
| accounts    | export | Caching, Ether, Wei |          |
| chain_state | state  | Caching, Ether, Wei |          |

### Group 4

These commands produce data, but because they do query the node, the need the cache. They also have wei values which are allowed to use `--ether` option to convert to ether. The `--raw` option shows the underlying data queried directly from the node.

| Group      | Cmd          | Enabled                  | Disabled |
| ---------- | ------------ | ------------------------ | -------- |
| chain_data | transactions | Caching, Ether, Wei, Raw |          |
| chain_data | blocks       | Caching, Ether, Wei, Raw |          |
| chain_data | receipts     | Caching, Ether, Wei, Raw |          |
| chain_data | logs         | Caching, Ether, Wei, Raw |          |
| chain_data | traces       | Caching, Ether, Wei, Raw |          |
| other      | slurp        | Caching, Ether, Wei, Raw |          |

### Group 5

These commands do not produce any really data. They are mostly used for configuration / long running processing. The `--file` option allows for automation which in most cases, doesn't apply. The `daemon` option does not accept `--chain`.

| Group | Cmd     | Enabled | Disabled                                   |
| ----- | ------- | ------- | ------------------------------------------ |
| admin | daemon  |         | Output, Append, Fmt, NoHeader, File, Chain |
| admin | scrape  |         | Output, Append, Fmt, NoHeader, File        |
| admin | init    |         | Output, Append, Fmt, NoHeader, File        |
| other | explore |         | Output, Append, Fmt, NoHeader              |
