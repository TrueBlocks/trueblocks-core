### Exporting Data from QuickBlocks

QuickBlocks allows you to extract and then export transactional data from a locally running Ethereum node. This document describes the export portion of QuickBlocks.

-

In this folder you will find subfolders labeled `group_01, group_02, ..., etc`. (A similar set of subfolders is in the `../monitors` subfolder).

Each `group` contains two things: (1) a file called `config.toml`, and (2) a subfolder called `cache`. Generally speaking, you should not to remove the cache. The cache can be recreated following the instructions below, but if you don't have a text file backup of the cache, you won't be able to re-create it. (Building the cache is the job of the `blockScrape` application which you do not have.)

The `config.toml` file contains information needed by the `acctScraper` application to extract transactions. It also instructs the `acctExport` application on what to export. (Like `blockScrape`, you do not have `acctScrape`.)

In order to generate the JSON (or tab-delimited, or comma-seperated values), follow these steps:

    cd ./quickBlocks/accounting/group_XX
    acctExport

(Assumming ./quickBlocks/bin is in your $PATH. If not, add it.)

When you run the above, you should see the JSON data for the addresses listed in the `config.toml` file. If you enter

    acctExport --fmt txt

You will get tab delimited data. You may also filter on specific addresses.

    acctExport --filter <addr>

-

`acctExport` has two options:

    --fmt <format>      Exports the data in the one of three formats [ json (default) | txt | csv ]
    --filter <addr>     If the config.toml file has more than one address, export only this addressess

**Note:** You may add more than one --filter on a given command line.

## Working with the cache

Sometimes you may need to remove a record from the list of transactions for a monitor. Alternatively, you may need to add new records. You do this with the `cacheMan` (cache manager) program [which is described here](cacheMan.md).

## Configuration

There are many configuration options available (all of which you may change by editing the `config.toml` file). The options available are [documented here](./CONFIG.md).
