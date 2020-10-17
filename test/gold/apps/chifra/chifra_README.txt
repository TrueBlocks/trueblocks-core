chifra argc: 2 [1:-th] 
chifra -th 
#### Usage

`Usage:`    chifra [-s|-v|-h] commands  
`Purpose:`  Main TrueBlocks command line controls.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | commands | which command to run, one or more of [list&#124;export&#124;slurp&#124;collections&#124;names&#124;abi&#124;state&#124;tokens&#124;when&#124;data&#124;blocks&#124;transactions&#124;receipts&#124;logs&#124;traces&#124;quotes&#124;scrape&#124;status&#124;rm] (required) |
| -s | --sleep <num> | for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 14) |

#### Hidden options (shown during testing only)
| -S | --start <num> | first block to process (inclusive) |
| -E | --end <num> | last block to process (inclusive) |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Valid commands for chifa are noted here. Get additional help with `'chifra <cmd> --help'`.
 MONITORS
   list          list all appearances of an address anywhere on the chain.
   export        export every appearance (as a transaciton, receipt, log, balance, etc.)
   rm            remove or pause a monitored address.
   slurp         query EtherScan for a list of transactions (note: will be smaller than --list)
 SHARED DATA
   scrape        scrape the blockchain and build the TrueBlocks address index (i.e. the digests).
   names         list all names known by TrueBlocks.
   abi           list all abi signatures known by TrueBlocks.
 BLOCKCHAIN DATA
   blocks        query the blockchain for block data
   transactions  query the blockchain for transaction data
   logs          query the blockchain for log data
   receipts      query the blockchain for receipt data
   traces        query the blockchain for trace data
   state         query the blockchain for the state of an address.
   tokens        query the blockchain for the state of an ERC20 address.
 OTHER
   status        query for various status reports about the system.
 

