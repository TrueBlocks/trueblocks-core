isContract argc: 2 [1:-th] 
isContract -th 
#### Usage

`Usage:`    isContract [-d|-n|-v|-h] addr[s]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) hold byte code (or displays the code).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr[s] | a list of Ethereum addresses from which to check for byte code |
| -d | --display | display the actual code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
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

#### Powered by QuickBlocks&reg;
