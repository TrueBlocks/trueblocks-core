getBalance argc: 2 [1:-th] 
getBalance -th 
#### Usage

`Usage:`    getBalance [-d|-e|-v|-h] addrs blocks  
`Purpose:`  Retrieve the balance for an account at a block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addrs | Ethereum address (starting with '0x') from which to retrieve the balance |
|  | blocks | the block at which to retrieve the balance (defaults to 'latest') |
| -d | --data | render results as tab delimited data |
| -e | --ether | return the balance in Ether instead of Wei |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

