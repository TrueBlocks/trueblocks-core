---
title: "Accounts"
description: "Access and cache transactional data"
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
weight: 11000
toc: true
---

The Accounts group of commands is at the heart of TrueBlocks. They allow you to produce and analyze
transactional histories for one or more Ethereum addresses.

You may also name addresses; grab the ABI file for a given address; add, delete, and remove
monitors, and, most importantly, export transactional histories in various formats, This
includes re-directing output to remote or local databases.

To the right is a list of commands in this group. Click on a command to see its full documentation.

## chifra list

`chifra list` takes one or more addresses, queries the index of appearances, and builds TrueBlocks
monitors. A TrueBlocks monitor is a file that contains blockNumber.transactionIndex pairs (transaction
identifiers) representing the history of the address.

Because TrueBlocks only extracts data from the Ethereum node when it's requested, the first time
you list an address it takes about a minute. Subsequent queries are much faster because TrueBlocks
caches the results.

Note that `chifra list` only queries the index, it does not extract the full transactional details.
You may use `chifra export` for that.

```[plaintext]
Purpose:
  List every appearance of an address anywhere on the chain.

Usage:
  chifra list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)

Flags:
  -U, --count               display only the count of records for each monitor
  -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
  -b, --bounds              report first and last block this address appears
  -u, --unripe              list transactions labeled unripe (i.e. less than 28 blocks old)
  -s, --silent              freshen the monitor only (no reporting)
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 250)
  -E, --reversed            produce results in reverse chronological order
  -F, --first_block uint    first block to export (inclusive, ignored when freshening)
  -L, --last_block uint     last block to export (inclusive, ignored when freshening)
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - No other options are permitted when --silent is selected.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [bounds](/data-model/accounts/#bounds)
- [monitor](/data-model/accounts/#monitor)

Links:

- [api docs](/api/#operation/accounts-list)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/list)

## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using
the index of appearances created with `chifra scrape` and the list of transaction identifiers
created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional
history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's
`source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`,
`--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export
the results to any database (with a little bit of work). The format of the data, its content and
its destination are up to you.

```[plaintext]
Purpose:
  Export full details of transactions for one or more addresses.

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
  -n, --neighbors           export the neighbors of the given address
  -A, --statements          export only statements
  -X, --transfers           export only eth or token transfers
  -s, --assets              list all assets (with names) that appear in any transfer
  -b, --balances            traverse the transaction history and show each change in ETH balances
  -i, --withdrawals         export withdrawals for the given address
  -a, --articulate          articulate transactions, traces, logs, and outputs
  -R, --cache_traces        force the transaction's traces into the cache
  -U, --count               for --appearances mode only, display only the count of records
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 250)
  -N, --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
  -m, --emitter strings     for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings       for the --logs option only, filter logs to show only those with this topic(s)
  -T, --nfts                for the --logs option only, filter logs to show only nft transfers
  -V, --reverted            export only transactions that were reverted
  -P, --asset strings       export transfers, balances, or statements only for this asset
  -f, --flow string         export transfers, balances, or statements with incoming, outgoing, or zero value
                            One of [ in | out | zero ]
  -y, --factory             for --traces only, report addresses created by (or self-destructed by) the given address(es)
  -u, --unripe              export transactions labeled unripe (i.e. less than 28 blocks old)
  -E, --reversed            produce results in reverse chronological order
  -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
  -F, --first_block uint    first block to process (inclusive)
  -L, --last_block uint     last block to process (inclusive)
  -C, --accounting          deprecated option, you may simply remove it
  -H, --ether               specify value in ether
  -o, --cache               force the results of the query into the cache
  -D, --decache             removes related items from the cache
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.
  - If present, --first_/--last_block are applied, followed by user-supplied filters such as asset or topic, followed by --first_/--max_record if present.
  - The --first_record and --max_record options are zero-based (as are the block options).
  - The _block and _record filters are ignored when used with the --count option.
  - If the --reversed option is present, the appearance list is reversed prior to all processing (including filtering).
  - The --decache option will remove all cache items (blocks, transactions, traces, etc.) for the given address(es).
  - The --withdrawals option is only available on certain chains. It is ignored otherwise.
  - The --traces option requires your RPC to provide trace data. See the README for more information.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [function](/data-model/other/#function)
- [log](/data-model/chaindata/#log)
- [message](/data-model/other/#message)
- [monitor](/data-model/accounts/#monitor)
- [parameter](/data-model/other/#parameter)
- [receipt](/data-model/chaindata/#receipt)
- [statement](/data-model/accounts/#statement)
- [token](/data-model/chainstate/#token)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [transaction](/data-model/chaindata/#transaction)
- [transfer](/data-model/accounts/#transfer)
- [withdrawal](/data-model/chaindata/#withdrawal)

Links:

- [api docs](/api/#operation/accounts-export)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/export)

### further information

The `--traces` option requires your node to enable the `trace_block` (and related) RPC endpoints. Please see the README file for the `chifra traces` command for more information.

## chifra monitors

`chifra monitors` has allows you to display information about the current set of monitors. This is
not to be confused with the monitoring function available from the `khedra` tool. Please see that
[Khedra Book for more information](https://khedra.trueblocks.io/).

### Crud commands

`chifra list` creates a new monitor. See that tool's help file for more information.

The `chifra monitors --delete` command deletes (or `--undelete` if already deleted) an address but does not remove it from your hard drive. The monitor is marked as being deleted, making it invisible to other tools.

Use the `--remove` command to permanently remove a monitor from your computer. This is an
irreversible operation and requires the monitor to have been previously deleted.

The `--decache` option will remove not only the monitor but all of the cached data associated with
the monitor (for example, transactions or traces). This is an irreversible operation (except
for the fact that the cache can be easily re-created with `chifra list <address>`). The monitor need not have been previously deleted.

```[plaintext]
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process

Flags:
      --delete       delete a monitor, but do not remove it
      --undelete     undelete a previously deleted monitor
      --remove       remove a previously deleted monitor
  -C, --clean        clean (i.e. remove duplicate appearances) from monitors, optionally clear stage
  -l, --list         list monitors in the cache (--verbose for more detail)
  -c, --count        show the number of active monitors (included deleted but not removed monitors)
  -S, --staged       for --clean, --list, and --count options only, include staged monitors
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all existing monitors will be cleaned.
```

Data models produced by this tool:

- [message](/data-model/other/#message)
- [monitor](/data-model/accounts/#monitor)
- [monitorclean](/data-model/admin/#monitorclean)

Links:

- [api docs](/api/#operation/accounts-monitors)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/monitors)

## chifra names

`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum
addresses. One may ask why this is necessary given that ENS exists. The answer is a single
word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses
private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of
other tools, especially `chifra export` where naming addresses becomes the single best way to
turn unintelligible blockchain data into understandable information.

The various options allow you to search and filter the results. The `tags` option is used primarily
by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

```[plaintext]
Purpose:
  Query addresses or names of well-known accounts.

Usage:
  chifra names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)

Flags:
  -e, --expand            expand search to include all fields (search name, address, and symbol otherwise)
  -m, --match_case        do case-sensitive search
  -a, --all               include all (including custom) names in the search
  -c, --custom            include only custom named accounts in the search
  -p, --prefund           include prefund accounts in the search
  -s, --addr              display only addresses in the results (useful for scripting, assumes --no_header)
  -g, --tags              export the list of tags and subtags only
  -C, --clean             clean the data (addrs to lower case, sort by addr)
  -r, --regular           only available with --clean, cleans regular names database
  -d, --dry_run           only available with --clean or --autoname, outputs changes to stdout instead of updating databases
  -A, --autoname string   an address assumed to be a token, added automatically to names database if true
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.
```

Data models produced by this tool:

- [message](/data-model/other/#message)
- [name](/data-model/accounts/#name)

Links:

- [api docs](/api/#operation/accounts-names)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/names)

## chifra abis

The `chifra abis` tool retrieves one or more ABI files for the given address(es). It searches
for ABIs, sequentially, in the following locations:

- the current working folder,
- the TrueBlocks local cache,
- Etherscan,
- (in the future) ENS and Sourcify.

While this tool may be used from the command line, its primary purpose is in support of
the `--articulate` option for tools such as `chifra export` and `chifra logs`.

If possible, the tool will follow proxied addresses searching for the ABI, but that does not
always work. In that case, you may use the `--proxy_for` option.

The `--known` option prints a list of semi-standard function signatures such as the ERC20 standard,
ERC 721 standard, various functions from OpenZeppelin, various Uniswap functions, etc. As an
optimization, the `known` signatures are searched first during articulation.

The `--encode` option generates a 32-byte encoding for a given canonical function or event signature. For
functions, you may manually extract the first four bytes of the hash.

The `--find` option is experimental. Please see the notes below for more information.

```[plaintext]
Purpose:
  Fetches the ABI for a smart contract.

Usage:
  chifra abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)

Flags:
  -k, --known              load common 'known' ABIs from cache
  -r, --proxy_for string   redirects the query to this implementation
  -l, --list               a list of downloaded abi files
  -i, --list_funcs         a list of the functions in all abi files
  -I, --list_events        a list of the events in all abi files
  -c, --count              show the number of abis downloaded
  -f, --find strings       search for function or event declarations given a four- or 32-byte code(s)
  -n, --hint strings       for the --find option only, provide hints to speed up the search
  -e, --encode string      generate the 32-byte encoding for a given canonical function or event signature
  -o, --cache              force the results of the query into the cache
  -D, --decache            removes related items from the cache
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose output
  -h, --help               display this help screen

Notes:
  - Search for either four byte signatures or event signatures with the --find option.
```

Data models produced by this tool:

- [abi](/data-model/other/#abi)
- [function](/data-model/other/#function)
- [parameter](/data-model/other/#parameter)

Links:

- [api docs](/api/#operation/accounts-abis)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/abis)

### further information

Without the --verbose option, the result is a compacted form of the ABI. Add --verbose for full details.

The `chifra abis --find` option scans the cross product of two sets. The first set contains more than 100,000 function and event
names. The second set contains approximately 700 function signatures. The cross product of these two sets creates 70,000,000
combinations of name(signature) each of which is hashed to create either a four-byte or a 32-byte hash. Very infrequently,
the tool will find matches for an otherwise unknown signatures.

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
