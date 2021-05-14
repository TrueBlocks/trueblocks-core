chifra argc: 2 [1:-th] 
chifra -th 
### Usage

`Usage:`    chifra command  
`Purpose:`  Access to all TrueBlocks tools (`chifra <cmd> --help` for more).

`Where:`  
```
 ACCOUNTS
   list          list every appearance of an address anywhere on the chain
   export        export details for each appearance (as txs, logs, traces, etc.)
   monitors      add, remove, clean, and list appearances of address(es)
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
   status        query the status of the system
   serve         serve the TrueBlocks API via the flame server
   scrape        scrape the chain and build an index of address appearances (aka digests)
   init          initialize TrueBlocks databases by downloading pinned bloom filters
   pins          query the status of the pinning system
 OTHER
   quotes        return prices collected from configured remote API
   explore       open the configured block explorer for the given address
   slurp         export details by querying EtherScan
 
```

