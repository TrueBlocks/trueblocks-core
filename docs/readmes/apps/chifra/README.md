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
   export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)
   monitor       add, remove, clean, and list appearances of address(es) on the chain
   names         list and/or share named addresses
   abis          list and/or share abi signatures
 CHAIN DATA
   blocks        export block-related data
   transactions  export transaction-related data
   receipts      export receipt-related data
   logs          export log-related data
   traces        export trace-related data
   when          return a date given a block number or a block number given a date
 CHAIN STATE
   state         export parts of the state for given address(es)
   tokens        export data related to ERC20 and/or ERC721 token(s)
 ADMIN
   init          initialize TrueBlocks databases by downloading pinned bloom filters
   status        query the status of the system
   scrape        scrape the chain and build an index of address appearances (aka digests)
   serve         serve the TrueBlocks API via the flame server
   pins          query the status of the pinning system
 OTHER
   explore       open the configured block explorer for the given address
   slurp         export details by querying EtherScan (note: will not return as many appearances as --list)
   quotes        return prices collected from configured remote API
```

**Source code**: [`apps/chifra`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra)

