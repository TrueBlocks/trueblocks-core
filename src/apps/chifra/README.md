## chifra

***
**Note:** This tool is available through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

`chifra` is an command line tool that builds [TrueBlocks account monitors](../../monitors/README.md) through a series of interactive questions.

Given your answers to those questions (such as which Ethereum addresses to monitor, what to name those addresses, where to find ABI files for those addresses), `chifra` generates the C++ code for both the parsing library needed to monitor the addresses and the code to build an interactive transactional debugging/auditing program.

Prior to running the program, collect together three pieces of information for each address you wish to monitor. You may monitor as many addresses as you wish. The information you need per address is:

- an Ethereum address
- a name to assign to this address (you may pick up the name using [ethName](../../tools/ethName/README.md))
- the block number when this address was first used, or (if a smart contract) when it was deployed.

`Chifra` uses [makeClass](../makeClass/README.md) and [grabABI](../grabABI/README.md) to build the monitors.

Here is a screen shot of an interaction with Chifra building a smart contract monitor for a particular token.

<img src=docs/image.png>

#### Usage

`Usage:`    chifra [-s|-v|-h] commands  
`Purpose:`  Main TrueBlocks command line controls.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | commands | which command to run, one or more of [list&#124;export&#124;slurp&#124;collections&#124;names&#124;tags&#124;abis&#124;blocks&#124;transactions&#124;receipts&#124;logs&#124;traces&#124;quotes&#124;state&#124;tokens&#124;when&#124;where&#124;dive&#124;scrape&#124;status&#124;rm] (required) |
| -s | --sleep <num> | for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 14) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Get more detailed help with `'chifra <cmd> --help'`.
 MONITORS
   list          list all appearances of address(es) on the chain, also adds monitor(s)
   export        export details for each appearance (as transacitons, logs, traces, balances, etc.)
   slurp         export details by querying EtherScan (note: will not return as many appearances as --list)
   rm            remove previously monitored address(es).
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
   scrape        scrape the chain and build an index of address appearances (aka digests).
   status        query the status of the system
   quotes        return prices collected from configured remote API
   when          return a date given a block number or a block number given a date
   where         determine the location of block(s), either local or remote cache
 

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

**Powered by TrueBlocks<sup>&trade;</sup>**


