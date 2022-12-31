---
title: "Configurations"
description: ""
lead: ""
date: 2021-11-22T19:18:35
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
weight: 2100
toc: true
---
Many of the `chifra` commands allow you to customize their behaviour through configuration files
and/or environment variables. These options are documented here.

### Environment Variables

Each command-line option may be overridden by exporting an environment variable in your shell prior
to running a `chifra` command.

The name of those environment variables is as follows:

1. The environment variable is `ALL_UPPER_CASE`
2. The environment variable name preserves underbars in the `item_name`
3. The name of the variable starts with the `group` the item belongs to

For example: `GROUP_ITEM_NAME`.

A more concrete example might be:

```[shell]
export SETTINGS_RPCPROVIDER=http://localhost:9876
chifra blocks 100
```

which would cause `chifra` to use an alternate rpcProvider without having to edit the
configuration file.

This feature comes in handy when build shell scripts to automate various tasks with `chifra`.

### Where Are Configuration Files Stored?

Please see [TODO: PLACE_HODLER](#) for more information.

### Separate Files

A single global configuration, called `trueBlocks.toml`, is stored at the root of the configuration
folder.

In addition, each individual tool has its own configuration file with items peculuar to that tool.
If a configuration item is found in a particular file, it applies only to that tool.

If, however, one of the items documented below under `trueBlocks.toml` is found in a tool's
individual config, it will override that value for that tool only.

For historical reasons, the configuration files are names based on old tool names. Please see
the table below for the name of each tool's config file.

### Multichain

If you're running against mutliple chains, you may place any of these files in the root of the
chain's configuration folder, and the values found there will replace any values found at the
top level. In this way, you may configure all chains for certain values, but customize your
configuration per chain.

### Configuration Files

<div style="padding:2px;padding-left:10px;background-color:green;color:white">trueBlocks.toml (all tools)</div>

| Item          | Description / Default                                  |
| ------------- | ------------------------------------------------------ |
|               |                                                        |
| [settings]    |                                                        |
| rpcProvider   | The RPC endpoint (required)<br />http://localhost:8545 |
| cachePath     | Location of binary cache<br />$CONFIG/cache/           |
| indexPath     | Location of unchained index<br />$CONFIG/unchained/    |
| etherscan_key | API key for EtherScan (optional)<br/>empty             |
|               |                                                        |
| [dev]         |                                                        |
| debug_curl    | Increases log level for curl commands<br />false       |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">All tools (in each file)</div>

| Item      | Description / Default                       |
| --------- | ------------------------------------------- |
| [display] |                                             |
| format    | Customizes default output of tool<br/>empty |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">acctExport.toml for chifra export</div>

| Item                | Description / Default                                                                                                               |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------- |
|                     |                                                                                                                                     |
| [settings]          |                                                                                                                                     |
| cache               | If true, queried transactions are cached<br />false                                                                                 |
| cache_traces        | If true, queried traces are cached<br />false                                                                                       |
| ether_rounding      | When doing reconciliations, round to this number of decimals places<br />18                                                         |
| start_when_deployed | For smart contracts only, if `true`, start scan when contract is first deployed. If `false`, scan entire history (slower)<br />true |
| max_traces          | For any given transaction, limit number of traces to this number<br />250                                                           |
| skip_ddos           | Skip over 2016 dDos attacks<br />true                                                                                               |
|                     |                                                                                                                                     |
| [display]           |                                                                                                                                     |
| format              | Display format when exporting transactions (search source code)<br />STR_DISPLAY_TRANSACTION                                        |
| receipt             | Display format when exporting receipts<br />STR_DISPLAY_RECEIPT                                                                     |
| log                 | Display format when exporting logs<br />STR_DISPLAY_LOGENTRY                                                                        |
| trace               | Display format when exporting traces<br />STR_DISPLAY_TRACE                                                                         |
| appearances         | Display format when exporting appearances<br />STR_DISPLAY_APPEARANCEDISPLAY                                                        |
| neighbor            | Display format when exporting neighbors<br />STR_DISPLAY_APPEARANCE                                                                 |
| statement           | Display format when exporting statements<br />STR_DISPLAY_RECONCILIATION                                                            |
|                     |                                                                                                                                     |
| [exclusions]        |                                                                                                                                     |
| enabled             | If `true`, exclude addresses in `list` from extractions<br />false                                                                  |
| list                | If `[exclusions]enabled` is `true`, exclude this list of addresses from extractions<br />empty                                      |

<a href="#C4"></a>
<div style="padding:2px;padding-left:10px;background-color:green;color:white">blockScrape.toml for chifra scrape</div>

**Configuration group:** `[settings]`  

| Item           | Type   | Default | Description / Default                                                                                                    |
| -------------- | ------ | ------- | ------------------------------------------------------------------------------------------------------------------------ |
|                |        |
| [settings]     |        |
| apps_per_chunk | uint64 | 200000  | the number of appearances to build into a chunk before consolidating it                                                  |
| snap_to_grid   | uint64 | 100000  | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| first_snap     | uint64 | 0       | the first block at which snap_to_grid is enabled                                                                         |
| unripe_dist    | uint64 | 28      | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe                |
| channel_count  | uint64 | 20      | number of concurrent processing channels                                                                                 |
| allow_missing  | bool   | false   | do not report errors for blockchains that contain blocks with zero addresses                                             |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">chunkMan.toml for chifra chunks</div>

| Item              | Description / Default                                              |
| ----------------- | ------------------------------------------------------------------ |
|                   |                                                                    |
| [enabled]         |                                                                    |
| download_manifest | If true, download the index manifest prior to processing<br />true |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">getTraces.toml for chifra traces</div>

| Item       | Description / Default                                                                 |
| ---------- | ------------------------------------------------------------------------------------- |
|            |                                                                                       |
| [settings] |                                                                                       |
| skip_ddos  | If true, skip over September 2016 dDos transactions<br />true                         |
| max        | If `skip_ddos` is `true`, this number of traces defines a 'dDos' transaction<br />250 |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">ethslurp.toml for chifra slurp</div>

*See the source code for information on customizing this tool -- this legacy code does not comply with other tools.*

### Other tools

The following tools are documented, but customizing them is not supported. If you change something here, and you break your installation, please don't tell us we didn't warn you.

<div style="padding:2px;padding-left:10px;background-color:green;color:white">testRunner.toml for testRunner</div>

| Item              | Description / Default                                                                                       |
| ----------------- | ----------------------------------------------------------------------------------------------------------- |
|                   |                                                                                                             |
| [settings]        |                                                                                                             |
| api_provider      | The location of the API endpoint gotten from `chifra daemon`<br />http://localhost:8080                     |
| run_local         | If true, run tests labeled as `local` (most will fail)<br />false                                           |
| json_pretty_print | Post processor for API test cases (formats output to verify valid JSON production)<br />jq .                |
| test_epoch        | The epoch to report in the performance testing tool - usually tracks current version<br />E-<VERSION_MINOR> |
| copy_path         | An alternate path to which to copy performance results                                                      |

<div style="padding:2px;padding-left:10px;background-color:green;color:white">makeClass.toml for makeClass</div>

| Item        | Description / Default                                                                                                |
| ----------- | -------------------------------------------------------------------------------------------------------------------- |
|             |                                                                                                                      |
| [settings]  |                                                                                                                      |
| disabled    | If true, makeClass is enabled. Be warned -- this will overwrite source code file if improperly configured<br />false |
| last_format | The timestamp at date the makeClass formatter was run<br />0                                                         |
| last_lint   | The timestamp at the date the makeClass linter was run<br />0                                                        |
|             |                                                                                                                      |
| [enabled]   |                                                                                                                      |
| auto_format | If true, run the auto-formatter<br />false                                                                           |
| generate    | If true, run the auto-code generator<br />false                                                                      |
| readmes     | If true, generate README files with `make generate`<br />false                                                       |
| lint_all    | If true, run the linter when doing `make finish`<br />false                                                          |
| tsx         | (deprecated)<br />false                                                                                              |

The follow values are defined for each classDefinition file

| Item         | Description / Default                                              |
| ------------ | ------------------------------------------------------------------ |
|              |                                                                    |
| [settings]   |                                                                    |
| class        | The name of the C++ class                                          |
| base_class   | The baseclass for class<br />CBaseNode                             |
| fields       | The name of the file holding the field definitions for the class   |
| includes     | The include files for the class's header file                      |
| cpp_includes | The include files for the class's c++ file                         |
| sort         | If true, add sorting function to the class                         |
| equals       | If true, add equality function to the class                        |
| contained_by | If present, the name of the class that contains items of this type |
| doc_producer | The name of the chifra tool that produces this type of data        |
| tsx          | (deprecated)<br />false                                            |
| doc_group    | The documentation group this class belongs to                      |
| doc_descr    | The description of the class for the documentation                 |
| doc_api      | The API something or other for something or other                  |
| force_objget | Some flag for some reason<br />false                               |
