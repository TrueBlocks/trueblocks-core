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

### usage

`Usage:`    chifra command  
`Purpose:`  Access to all TrueBlocks tools (`chifra <cmd> --help` for more).

`Where:`
```
 ACCOUNTS
   list          list every appearance of an address anywhere on the chain
   export        export full detail of transactions for one or more addresses
   monitors      add, remove, clean, and list address monitors
   names         query addresses or names of well known accounts
   abis          fetches the ABI for a smart contract
 CHAIN DATA
   blocks        retrieve one or more blocks from the chain or local cache
   transactions  retrieve one or more transactions from the chain or local cache
   receipts      retrieve receipts for the given transaction(s)
   logs          retrieve logs for the given transaction(s)
   traces        retrieve traces for the given transaction(s)
   when          find block(s) based on date, blockNum, timestamp, or 'special'
 CHAIN STATE
   state         retrieve account balance(s) for one or more addresses at given block(s)
   tokens        retrieve token balance(s) for one or more addresses at given block(s)
 ADMIN
   status        report on the status of the TrueBlocks system
   serve         serve the TrueBlocks API using the flame server
   scrape        scan the chain and update the TrueBlocks index of appearances
   init          initialize the index of appearances by downloading Bloom filters
   pins          manage pinned index of appearances and associated Bloom filters
 OTHER
   quotes        freshen and/or display Ethereum price data
   explore       open an explorer for a given address, block, or transaction
   slurp         fetch data from EtherScan for any address
 
```

**Source code**: [`apps/chifra`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra)

