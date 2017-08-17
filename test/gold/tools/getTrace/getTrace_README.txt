getTrace argc: 2 [1:-th] 
getTrace -th 
#### Usage

`Usage:`    getTrace [-t|-r|-v|-h] hash  
`Purpose:`  Retrieve the full trace of a transaction.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | hash | transaction hash of the requested transaction |
| -t | --terse | display the traces in a terse fashion |
| -r | --raw | return the trace directly from the node without processing |
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
