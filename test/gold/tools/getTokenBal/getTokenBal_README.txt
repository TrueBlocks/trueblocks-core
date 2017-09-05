getTokenBal argc: 2 [1:-th] 
getTokenBal -th 
#### Usage

`Usage:`    getTokenBal [-d|-e|-v|-h] token holders blocks  
`Purpose:`  Retrieve the token balance from a token contract for an account at a block (or more).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | token | token contract (starting with '0x') from which to retrieve the balance |
|  | holders | holder address(es) (starting with '0x') for which to retrieve the balance(s) |
|  | blocks | the block at which to retrieve the token balance(s) (defaults to 'latest') |
| -d | --data | render results as tab delimited data |
| -e | --ether | return the token balance in Ether instead of Wei |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

