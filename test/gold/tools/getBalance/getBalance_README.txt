getBalance argc: 2 [1:-th] 
getBalance -th 
#### Usage

`Usage:`    getBalance [-l|-n|-d|-v|-h] <address> [address...] [block...]  
`Purpose:`  Retrieve the ether balance(s) for one or more addresses at the latest or a list of blocks.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | address_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | optional list of one or more blocks at which to report balances, if empty defaults to 'latest' |
| -l | --list fn | an alternative way to specify an address_list. Place one address per line in the file 'fn' |
| -n | --noZero | suppress the display of zero balance accounts |
| -d | --data | render results as tab delimited data |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

