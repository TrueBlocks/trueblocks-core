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
weight: 71000
toc: true
---
## Global Options

Every `chifra` command has the following globally available options (unless the option is removed as shown in the `Disabled` column below). Certain commands have additional globally available features as noted in the `Enabled` column.

| Globally Options Available to All Commands                         |
| ------------------------------------------------------------------ |
| Verbose, Fmt, Version, Chain, NoHeader, File, Output, Append, Help |

Where:

  -v, --verbose            enable verbose output
  -x, --fmt string         export format, one of [none|json*|txt|csv]
      --version            displays the current version string
      --chain              instructs the tool to operate against the given chain
      --no_header          suppresses the display of the header in txt and csv format
      --file               reads options from the specified file
      --output             redirects output to the given file
      --append             for --output only, appends results to the given file
  -h, --help               displays the help screen

### Group 1

The tools in this group of commands produce data but do not need the cache because they do not query the node. They all have the above globally available options.

| Group    | Cmd      | Enabled | Disabled |
| -------- | -------- | ------- | -------- |
| accounts | list     |         |          |
| accounts | monitors |         |          |
| accounts | names    |         |          |
| admin    | config   |         |          |
| admin    | status   |         |          |
| admin    | chunks   |         |          |

### Group 2

The tools in this group have all of the above options, but they query the node, therefore they need the cache.

| Group       | Cmd    | Enabled        | Disabled |
| ----------- | ------ | -------------- | -------- |
| accounts    | abis   | Cache, Decache |          |
| chain_data  | when   | Cache, Decache |          |
| chain_state | tokens | Cache, Decache |          |

Where:

  -o, --cache              force the results of the query into the cache
  -D, --decache            removes related items from the cache

### Group 3

The tools in this group have all of the above options, but because they do query the node, they need the cache. They also produce wei values therefore they have the `--ether` option as well.

| Group       | Cmd    | Enabled               | Disabled |
| ----------- | ------ | --------------------- | -------- |
| accounts    | export | Cache, Decache, Ether |          |
| chain_state | state  | Cache, Decache, Ether |          |

Where:

  -H, --ether              specify value in ether

### Group 4

The tools in this group have all of the above options, but have an additional option, `--raw`, which shows the underlying data queried directly from the node (or API provider in the case of `slurp`).

| Group      | Cmd          | Enabled                    | Disabled |
| ---------- | ------------ | -------------------------- | -------- |
| chain_data | blocks       | Cache, Decache, Ether, Raw |          |
| chain_data | transactions | Cache, Decache, Ether, Raw |          |
| chain_data | receipts     | Cache, Decache, Ether, Raw |          |
| chain_data | logs         | Cache, Decache, Ether, Raw |          |
| chain_data | traces       | Cache, Decache, Ether, Raw |          |
| other      | slurp        | Cache, Decache, Ether, Raw |          |

Where:

      --raw                passes the data queried from the node directly to the display

### Group 5

This final group of tools do not produce any real data. They are mostly used for configuration and/or to start and stop long-running processes. They allow no additional options over the default, but disable the following options depending on context. The `deamon` option, which provides all the tools via a local API, disables `--chain` because one sends chain with the URL.

| Group | Cmd     | Enabled | Disabled                                   |
| ----- | ------- | ------- | ------------------------------------------ |
| admin | daemon  |         | Output, Append, Fmt, NoHeader, File, Chain |
| admin | scrape  |         | Output, Append, Fmt, NoHeader, File        |
| admin | init    |         | Output, Append, Fmt, NoHeader, File        |
| other | explore |         | Output, Append, Fmt, NoHeader              |
