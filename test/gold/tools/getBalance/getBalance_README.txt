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
