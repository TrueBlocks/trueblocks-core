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

#### Usage

`Usage:`    chifra command  
`Purpose:`  Access to all TrueBlocks tools (`chifra <cmd> --help` for more).

`Where:`  
```
 MONITORS
   list          list every appearance of an address anywhere on the chain
   export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)
   monitor       add, remove, clean, and list appearances of address(es) on the chain
 SHARED DATA
   entities      list and/or share entities (groups of addresses)
   names         list and/or share named addresses
   tags          list and/or share tags (subgroups of addresses)
   abis          list and/or share abi signatures
 BLOCKCHAIN DATA
   blocks        export block-related data
   transactions  export transaction-related data
   receipts      export receipt-related data
   logs          export log-related data
   traces        export trace-related data
   state         export parts of the state for given address(es)
   tokens        export data related to ERC20 and/or ERC721 token(s)
   when          return a date given a block number or a block number given a date
 ADMIN
   init          initialize TrueBlocks databases by downloading pinned bloom filters
   scrape        scrape the chain and build an index of address appearances (aka digests)
   serve         serve the TrueBlocks API via tbServer
   pins          query the status of the pinning system
   status        query the status of the system
 OTHER
   explore       open the configured block explorer for the given address
   slurp         export details by querying EtherScan (note: will not return as many appearances as --list)
   quotes        return prices collected from configured remote API
   where         determine the location of block(s), either local or remote cache, or on-chain
```

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

