---
title: "Accounts"
description: ""
lead: ""
date: 2021-11-18T22:52:20
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1100
toc: true
---
This group of commands is at the heart of TrueBlocks. They allow you to produce and analyze transactional histories for a given Ethereum address.

You may also name addresses; grab the ABI file for a given address; add, delete, and remove monitors, and, most importantly, export transactional histories to various formats,
This includes re-directing output to remote or local databases.
## chifra list

`chifra list` takes one or more addresses, queries the index of appearances, and builds TrueBlocks monitors. A TrueBlocks monitor is a file that contains blockNumber.transactionId pairs (transaction identifiers) representing the history of the address.

Because TrueBlocks only extracts data from the Ethereum node when it's requested, the first time you list an address it takes about a minute. Subsequent queries are much faster because TrueBlocks caches the results.

Note that `chifra list` only queries the index, it does not extract the full transactional details. You may use `chifra export` for that.

```[plaintext]
Purpose:
  List every appearance of an address anywhere on the chain.

Usage:
  chifra list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)

Flags:
  -U, --count        present only the number of records
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen
```

**Source code**: [`internal/list`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/list)

## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

```[plaintext]
Purpose:
  Export full detail of transactions for one or more addresses.

Usage:
  chifra export [flags] <address> [address...] [topics...] [fourbytes...]

Arguments:
  addrs - one or more addresses (0x...) to export (required)
  topics - filter by one or more log topics (only for --logs option)
  fourbytes - filter by one or more fourbytes (only for transactions and trace options)

Flags:
  -p, --appearances         export a list of appearances
  -r, --receipts            export receipts instead of transactional data
  -l, --logs                export logs instead of transactional data
  -t, --traces              export traces instead of transactional data
  -A, --statements          export reconciliations instead of transactional data (requires --accounting option)
  -n, --neighbors           export the neighbors of the given address
  -C, --accounting          attach accounting records to the exported data (applies to transactions export only)
  -a, --articulate          articulate transactions, traces, logs, and outputs
  -i, --cache               write transactions to the cache (see notes)
  -R, --cache_traces        write traces to the cache (see notes)
  -y, --factory             scan for contract creations from the given address(es) and report address of those contracts
  -U, --count               only available for --appearances mode, if present, return only the number of records
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process before reporting (default 250)
      --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
      --emitter strings     for log export only, export only logs if emitted by one of these address(es)
      --topic strings       for log export only, export only logs with this topic(s)
      --clean               clean (i.e. remove duplicate appearances) from all existing monitors
  -x, --fmt string          export format, one of [none|json*|txt|csv|api]
  -v, --verbose             enable verbose (increase detail with --log_level)
  -h, --help                display this help screen

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.
```

**Source code**: [`internal/export`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/export)

## chifra monitors

A TrueBlocks monitor is simply a file on your computer that represents the transactional history of a given Ethereum address. Monitors do not exist until you indicate your interest in a certain address. (See `chifra list`.)

You may use the `--delete` command to delete (or undelete if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversible operation.

```[plaintext]
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process (required)

Flags:
  -p, --appearances   export a list of appearances
  -U, --count         present only the number of records
      --clean         clean (i.e. remove duplicate appearances) from monitors
      --delete        delete a monitor, but do not remove it
      --undelete      undelete a previously deleted monitor
      --remove        remove a previously deleted monitor
  -x, --fmt string    export format, one of [none|json*|txt|csv|api]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all monitors will be cleaned.
```

**Source code**: [`internal/monitors`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/monitors)

## chifra names

`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum addresses. One may ask why this is necessary given that ENS exists. The answer is a single word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of other tools, especially `chifra export` where naming addresses becomes the single best way to turn unintelligible blockchain data into understandable information.

The various options allow you to search and filter the results. The `collections` and `tags` options are used primarily by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

```[plaintext]
Purpose:
  Query addresses or names of well known accounts.

Usage:
  chifra names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)

Flags:
  -e, --expand        expand search to include all fields (search name, address, and symbol otherwise)
  -m, --match_case    do case-sensitive search
  -l, --all           include all accounts in the search
  -c, --custom        include your custom named accounts
  -p, --prefund       include prefund accounts
  -n, --named         include well know token and airdrop addresses in the search
  -a, --addr          display only addresses in the results (useful for scripting)
  -s, --collections   display collections data
  -g, --tags          export the list of tags and subtags only
  -x, --fmt string    export format, one of [none|json*|txt|csv|api]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.
```

**Source code**: [`internal/names`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/names)

## chifra abis

`chifra abis` retrieves a smart contract's ABI file either from the current local folder, from the TrueBlocks cache, from [Etherscan](http://etherscan.io), or (in the future) from ENS and Sourcify.

While this tool may be used from the command line or the API, its primary use is in support of the `--articulate` option of other tools such as `chifra export`.

The `--known` option prints a list of standard and semi-standard function signatures such as the ERC20 standard, various OpenZeppelin functions, the base and pair Uniswap functions, etc.

The `--find` option is experimental. It scans a cross of approx. 100,000 function names with approx. 700 function signatures, generating at each cross a four byte signature and looking for a match to the provided fourbyte. Very infrequently, this tool will find a match for an otherwise unknown fourbyte string.

The `--sol` option will convert a single Solidity file found in the current folder into an ABI.

```[plaintext]
Purpose:
  Fetches the ABI for a smart contract.

Usage:
  chifra abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)

Flags:
  -k, --known          load common 'known' ABIs from cache
  -s, --sol            extract the abi definition from the provided .sol file(s)
  -f, --find strings   search for function or event declarations given a four- or 32-byte code(s)
  -x, --fmt string     export format, one of [none|json*|txt|csv|api]
  -v, --verbose        enable verbose (increase detail with --log_level)
  -h, --help           display this help screen

Notes:
  - For the --sol option, place the solidity files in the current working folder.
  - Search for either four byte signatures or event signatures with the --find option.
```

**Source code**: [`internal/abis`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/abis)

