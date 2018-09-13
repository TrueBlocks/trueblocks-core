## Account Monitors

The `acctScrape` program collects lists of transactions per account. These lists are stored in a minimal cache stored in the `./cache` folder that contains two values

    blockNumber.transctionIndex
    
Using `acctExport` one can then export full details of the transactions to `JSON`, `.txt`, or `.csv` files for use in data analysis.

This document describes how to use these tools.

### Where are the files

In the current folder, there are subfolders, each of which contains two things.

1. The cache stored in the folder `./cache`, and
2. The configuration file `config.toml`

Each monitor has its own cache and its own configuration file.

### Gathering Transactions per Account

QuickBlocks allows you to gather lists of transactions per account directly from a locally-running Ethereum node. 

To start an `acctScrape` monitor, do this:

    cd ./monitors/<monitor_name>
    acctScrape

where `<monitor_name>` is the folder containing the configuration file and binary cache for the monitor and we assume that `acctScrape` is in your `$PATH`. Details of the configuration file may be found in the example folders. The [acctScrape app is described here](../src/apps/acctScrape/README.md).

**Important Note:** The `acctScrape` application is currently **not publically available**. See the managing the binary cache section below for information on how to import lists of transactions from EtherScan.

### Exporting Transactions

Once you have a list of transactions in the cache, you may export it. Do so with the `acctExport` application, thus:

    cd ./monitors/<monitor_name>
    acctExport --fmt txt

If you've previously imported or scraped data into the given monitor (i.e. `<monitor_name>`), the above command will export to the screen. The default format is purposefully terse, but there are many options for expanding, filtering, and customizing the output. One simple way is to customize the output is to export JSON:

    cd ./monitors/<monitor_name>
    acctExport --fmt json

or just provide no --fmt option, as JSON is the default. The [acctExport application is described here](../src/apps/acctExport/README.md).

### Managing the Cache

You may wish to manage the cache. You may do using the `cacheMan` application. `cacheMan` allows you to import, remove, replace, list, and fix the data in a monitor. Type this command to get help:

    cd ./monitors/<monitor_name>
    cacheMan --help

As an example, if you've gathered a list of transactions from EtherScan, and have prepared an `import.txt` file (which contains simply one record for each transaction with two tab-separated fields `blockNumber` and `transactionIndex`), and placed that file in the monitor's folder, simply run:

    cd ./monitors/<monitor_name>
    cacheMan -i
    
If `cacheMan` finds a file in the current folder name `import.txt`, that data will be added to the cache. There's a similar feature for removing items from the cache. For more information, see the [cacheMan README.md](../src/apps/cacheMan/README.md) for more information.

### Configuration Files

There are many configuration options available (all of which may be changed by editing the `config.toml` file). The options available are [documented here](./CONFIG.md).
