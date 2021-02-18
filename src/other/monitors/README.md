## Account Monitors

The first and fundemental requirement of an Ethereum Account Monitor is to collect lists of transactions per account. This document describes how QuickBlocks can help you do that.

The present folder stores the configuration files and caches needed to create these lists of transactions from a locally running Ethereum node. Other tools allow you to gather those lists from third-party APIs such as EtherScan. Once you have a list of transactions for your account, then you can extract the full transactional details of the transactions directly from the node. This document describes how to accumulate account data from a node, how to manage the cache that stores the lists of transactions, and how to export the full data for use by other programs.

### Gathering Transactions per Account

QuickBlocks allows you to gather lists of transactions per account directly from a locally-running Ethereum node. Prior to QuickBlocks, the only way to gather such lists was to use a third party API such as EtherScan. This section discusses `acctExport` which provides lists of transactions per account from a locally-running node.

To start an `acctExport` monitor, do this:

    cd ./monitors/<monitor_name>
    acctExport

where `<monitor_name>` is the folder containing the configuration file and binary cache for the monitor. Details of the configuration file may be found in the example folders. The [acctExport app is described here](../src/apps/acctExport/README.md).

**Important Note:** The `acctExport` process is currently not publically available. See the managing the binary cache section below for information on how to import lists of transactions from EtherScan.

### Exporting Transactions

Once you have a list of transactions in the cache, you need to export it. You may do that with the `acctExport` application. Export transactions thus:

    cd ./monitors/<monitor_name>
    acctExport --fmt txt

If there is any data in the given monitor (i.e. `<monitor_name>`), this will export a list of transactions included in the cache including the `blockNumber, transactionId, hash` values. The default format is very terse, but ther are many options for enhancing the output. One simple way is to export to JSON:

    cd ./monitors/<monitor_name>
    acctExport --fmt json

or just leave the --fmt option off, as JSON is the default. The [acctExport app is described here](../src/apps/acctExport/README.md).

### Managing the Cache

Finally, you may want to manage the transaction cache for a particular monitor. You may do this with the `c acheMan` application. `c acheMan` allows you to import, remove, replace, list, and fix the data in a monitor. Type this to get help:

    cd ./monitors/<monitor_name>
    c acheMan

For example, if you've gathered a list of transactions from EtherScan, you may prepare an `import.txt` file (which contains simply one record with two tab-separated fields `blockNumber` and `transactionIndex` -- alternatively, simply the transaction hashes). Place that file in the monitor's folder and run:

    cd ./monitors/<monitor_name>
    c acheMan -i cache/0x.....bin
    
where `0x.....bin` is the name of the cache file. There's a similar feature for removing items from the cache. For more information, see the [c acheMan README.md](../src/apps/c acheMan/README.md) for more information.
