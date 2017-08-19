## ethslurp

ethslurp

Portions of this software are Powered by Etherscan.io APIs
#### Usage

`Usage:`    ethslurp [-a|-b|-d|-m|-n|-r|-f|-i|-e|-o|-l|-t|-v|-h] addr  
`Purpose:`  Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr | the address of the account or contract to slurp |
| -a | --archive | filename of output (stdout otherwise) |
| -b | --blocks | export records in block range (:0[:max]) |
| -d | --dates | export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]]) |
| -m | --max | maximum transactions to slurp (:250000) |
| -n | --name | name this address |
| -r | --rerun | re-run the most recent slurp |
| -f | --fmt | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -i | --income | include income transactions only |
| -e | --expense | include expenditures only |
| -o | --open | open the configuration file for editing |
| -l | --list | list previously slurped addresses in cache |
| -t | --test | generate intermediary files but do not execute the commands |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |         Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. Use semi-colon make comments.*

**Powered by QuickBlocks<sup>&reg;</sup>**

