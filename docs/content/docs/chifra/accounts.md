---
title: "Accounts"
description: ""
lead: ""
date: 2021-09-17T23:57:27
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

Becuase TrueBlocks only extracts data from the Ethereum node when it's requested, the first time you list an address it takes about a minute. Subsequent queries are much faster because TrueBlocks caches the results.

Note that `chifra list` only queries the index, it does not extract the full transactional details. You may use `chifra export` for that.

### usage

`Usage:`    chifra list &lt;address&gt; [address...]  
`Purpose:`  List appearances for the given address(es).

`Where:`

|          | Option                | Description                                        |
| -------- | --------------------- | -------------------------------------------------- |
|          | addrs                 | one or more addresses (0x...) to export (required) |
| &#8208;v | &#8208;&#8208;verbose | set verbose level (optional level defaults to 1)   |
| &#8208;h | &#8208;&#8208;help    | display this help screen                           |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)
## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

### usage

`Usage:`    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more addresses.

`Where:`

{{<td>}}
|          | Option                                 | Description                                                                                                    |
| -------- | -------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
|          | addrs                                  | one or more addresses (0x...) to export (required)                                                             |
|          | topics                                 | filter by one or more log topics (only for --logs option)                                                      |
|          | fourbytes                              | filter by one or more fourbytes (only for transactions<br/>and trace options)                                  |
| &#8208;p | &#8208;&#8208;appearances              | export a list of appearances                                                                                   |
| &#8208;r | &#8208;&#8208;receipts                 | export receipts instead of transaction list                                                                    |
| &#8208;A | &#8208;&#8208;statements               | for use with --accounting option only, export only<br/>reconciliation statements                               |
| &#8208;l | &#8208;&#8208;logs                     | export logs instead of transaction list                                                                        |
| &#8208;t | &#8208;&#8208;traces                   | export traces instead of transaction list                                                                      |
| &#8208;C | &#8208;&#8208;accounting               | export accounting records instead of transaction list                                                          |
| &#8208;a | &#8208;&#8208;articulate               | articulate transactions, traces, logs, and outputs                                                             |
| &#8208;i | &#8208;&#8208;cache_txs                | write transactions to the cache (see notes)                                                                    |
| &#8208;R | &#8208;&#8208;cache_traces             | write traces to the cache (see notes)                                                                          |
| &#8208;y | &#8208;&#8208;factory                  | scan for contract creations from the given address(es)<br/>and report address of those contracts               |
|          | &#8208;&#8208;emitter                  | for log export only, export only if one of the given<br/>export addresses emitted the event                    |
|          | &#8208;&#8208;source &lt;addr&gt;      | for log export only, export only one of these addresses<br/>emitted the event                                  |
|          | &#8208;&#8208;relevant                 | for log and accounting export only, if true export<br/>only logs relevant to one of the given export addresses |
| &#8208;U | &#8208;&#8208;count                    | only available for --appearances mode, if present return<br/>only the number of records                        |
| &#8208;c | &#8208;&#8208;first_record &lt;num&gt; | the first record to process                                                                                    |
| &#8208;e | &#8208;&#8208;max_records &lt;num&gt;  | the maximum number of records to process before reporting                                                      |
|          | &#8208;&#8208;clean                    | clean (i.e. remove duplicate appearances) from all<br/>existing monitors                                       |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;          | export format, one of [none, json, txt, csv, api]                                                              |
| &#8208;v | &#8208;&#8208;verbose                  | set verbose level (optional level defaults to 1)                                                               |
| &#8208;h | &#8208;&#8208;help                     | display this help screen                                                                                       |
{{</td>}}

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

`Configurable Items:`

`cache_txs`: write transactions to the cache (see notes).
`cache_traces`: write traces to the cache (see notes).
`skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).
`max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction
  is (default = 250).

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

## chifra monitors

A TrueBlock monitor is simply a file on your computer that represents the transactional history of a given Ethereum address. Monitors do not exist until you indicate your interest in a certain address. (See `chifra list`.)

You may use the `--delete` command to delete (or undelete if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversable operation.

### usage

`Usage:`    chifra monitors [-d|-r|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Delete, undelete, and remove previously created monitors.

`Where:`

|          | Option                | Description                                                          |
| -------- | --------------------- | -------------------------------------------------------------------- |
|          | addrs                 | one or more addresses (0x...) to export (required)                   |
|          | &#8208;&#8208;delete  | delete a previously created monitor (or undelete if already deleted) |
|          | &#8208;&#8208;remove  | remove a previously deleted monitor                                  |
| &#8208;v | &#8208;&#8208;verbose | set verbose level (optional level defaults to 1)                     |
| &#8208;h | &#8208;&#8208;-help   | display this help screen                                             |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)
## chifra names

`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum addresses. One may ask why this is necessary given that ENS exists. The answer is a single word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of other tools, especially `chifra export` where naming addresses becomes the single best way to turn unintellagable blockchain data into understandable information.

The various options allow you to search and filter the results. The `collections` and `tags` options are used primarily by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

### usage

`Usage:`    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses or names of well known accounts.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                               |
| -------- | ----------------------------- | ----------------------------------------------------------------------------------------- |
|          | terms                         | a space separated list of one or more search terms<br/>(required)                         |
| &#8208;e | &#8208;&#8208;expand          | expand search to include all fields (default searches<br/>name, address, and symbol only) |
| &#8208;m | &#8208;&#8208;match_case      | do case-sensitive search                                                                  |
| &#8208;l | &#8208;&#8208;all             | include all accounts in the search                                                        |
| &#8208;c | &#8208;&#8208;custom          | include your custom named accounts                                                        |
| &#8208;p | &#8208;&#8208;prefund         | include prefund accounts                                                                  |
| &#8208;n | &#8208;&#8208;named           | include well know token and airdrop addresses in the<br/>search                           |
| &#8208;a | &#8208;&#8208;addr            | display only addresses in the results (useful for scripting)                              |
| &#8208;s | &#8208;&#8208;collections     | display collections data                                                                  |
| &#8208;g | &#8208;&#8208;tags            | export the list of tags and subtags only                                                  |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                         |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                          |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                                  |
{{</td>}}

`Notes:`

- The tool will accept up to three terms, each of which must match against any field in the database.
- The `--match_case` option enables case sensitive matching.
- Name file: `$CONFIG/names/names.tab`

**Source code**: [`tools/ethNames`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethNames)

## chifra abis

`chifra abis` retrieves a smart contract's ABI file either from the current local folder, from the TrueBlocks cache, from [Etherscan](http://etherscan.io), or (in the future) from ENS and Sourcify.

While this tool may be used from the command line or the API, its primary use is in support of the `--articulate` option of other tools such as `chifra export`.

The `--known` option prints a list of standard and semi-standard function signatures such as the ERC20 standard, various OpenZeppelin functions, the base and pair Uniswap functions, etc.

The `--find` option is experimental. It scans a cross of approx. 100,000 function names with approx. 700 function signatures, generating at each cross a four byte signature and looking for a match to the provided fourbyte. Very infrequently, this tool will find a match for an otherwise unknown fourbyte string.

The `--sol` option will convert a single Solidity file found in the current folder into an ABI.

### usage

`Usage:`    chifra abis [-k|-s|-f|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract.

`Where:`

{{<td>}}
|          | Option                         | Description                                                                         |
| -------- | ------------------------------ | ----------------------------------------------------------------------------------- |
|          | addrs                          | list of one or more smart contracts whose ABI to grab<br/>from EtherScan (required) |
| &#8208;k | &#8208;&#8208;known            | load common 'known' ABIs from cache                                                 |
| &#8208;s | &#8208;&#8208;sol &lt;str&gt;  | file name of .sol file from which to create a new known<br/>abi (without .sol)      |
| &#8208;f | &#8208;&#8208;find &lt;str&gt; | try to search for a function declaration given a four<br/>byte code                 |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;  | export format, one of [none, json, txt, csv, api]                                   |
| &#8208;v | &#8208;&#8208;verbose          | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help             | display this help screen                                                            |
{{</td>}}

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

**Source code**: [`tools/grabABI`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/grabABI)

