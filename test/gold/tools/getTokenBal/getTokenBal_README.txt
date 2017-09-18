getTokenBal argc: 2 [1:-th] 
getTokenBal -th 
#### Usage

`Usage:`    getTokenBal [-b|-l|-n|-d|-v|-h] address_list block_list  
`Purpose:`  Retrieve token balances from a series of ERC20 token contracts for an account or visa versa.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | address_list | two or more address (0x...), the first is an ERC20 token, balances for the remaining accounts are reported |
|  | block_list | a list of one or more blocks at which to report balances, if empty defaults to 'latest' |
| -b | --byAcct | if enabled, all but the last address is considered an ERC20 token, balances for each for the last address are reported |
| -l | --list val | an alternative way to specify an address_list. One address per line |
| -n | --noZero | suppress the display of zero balance accounts |
| -d | --data | render results as tab delimited data (for example, to build a cap table) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

