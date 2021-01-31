chifra argc: 2 [1:-th] 
chifra -th 
#### Usage

`Usage:`    chifra [-v|-h] commands  
`Purpose:`  Main TrueBlocks command line controls.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | commands | which command to run, one or more of [list&#124;export&#124;slurp&#124;collections&#124;names&#124;tags&#124;abis&#124;blocks&#124;transactions&#124;receipts&#124;logs&#124;traces&#124;quotes&#124;state&#124;tokens&#124;when&#124;where&#124;dive&#124;init&#124;scrape&#124;serve&#124;status&#124;rm] (required) |
|####|Hidden options||
| -S | --start <num> | first block to process (inclusive) |
| -E | --end <num> | last block to process (inclusive) |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Get more detailed help with `'chifra <cmd> --help'`.
 MONITORS
   list          list all appearances of address(es) on the chain, also adds monitor(s)
   export        export details for each appearance (as transacitons, logs, traces, balances, etc.)
   slurp         export details by querying EtherScan (note: will not return as many appearances as --list)
   rm            remove previously monitored address(es)
 SHARED DATA
   collections   list and/or share collections (groups of addresses)
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
 OTHER
   init          initialize TrueBlocks databases
   scrape        scrape the chain and build an index of address appearances (aka digests)
   serve         serve the TrueBlocks API via goServer
   status        query the status of the system
   quotes        return prices collected from configured remote API
   when          return a date given a block number or a block number given a date
   where         determine the location of block(s), either local or remote cache

