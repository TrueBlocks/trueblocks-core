## NOTICE

This document is out of date. Please see our [website](http://trueblocks.io).

## chifra

`chifra` is an command line tool that builds [TrueBlocks account monitors](../../monitors/README.md) through a series of interactive questions.

Given your answers to those questions (such as which Ethereum addresses to monitor, what to name those addresses, where to find ABI files for those addresses), `chifra` generates the C++ code for both the parsing library needed to monitor the addresses and the code to build an interactive transactional debugging/auditing program.

Prior to running the program, collect together three pieces of information for each address you wish to monitor. You may monitor as many addresses as you wish. The information you need per address is:

- an Ethereum address
- a name to assign to this address (you may pick up the name using [ethName](../../tools/ethName/README.md))
- the block number when this address was first used, or (if a smart contract) when it was deployed.

`chifra` uses [chifra makeClass](../makeClass/README.md) and [chifra abis](../grabABI/README.md) to build the monitors.

Here is a screen shot of an interaction with Chifra building a smart contract monitor for a particular token.

<img src=docs/image.png>

Purpose: 
  Access to all TrueBlocks tools (`chifra <cmd> --help` for more).

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
    scrape        scan the chain and update the TrueBlocks index of appearances
    init          initialize the TrueBlocks system by downloading from IPFS
    pins          manage pinned index of appearances and associated blooms
  Other:
    quotes        freshen or display Ethereum price data. This tool has been deprecated
    explore       open an explorer for a given address, block, or transaction
    slurp         fetch data from EtherScan for any address
  Flags:
    -h, --help    display this help screen

  Use "chifra [command] --help" for more information about a command.

#### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --wei       | export values in wei (the default)                            |
| --ether     | export values in ether                                        |
| --dollars   | export values in US dollars                                   |
| --raw       | pass raw RPC data directly from the node with no processing   |
| --to_file   | write the results to a temporary file and return the filename |
| --output fn | write the results to file 'fn' and return the filename        |
| --file fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file fn` option, you may place a series of valid command lines in a file using any of the above flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes that line a comment.*</small>
