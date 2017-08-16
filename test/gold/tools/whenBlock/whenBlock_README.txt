whenBlock argc: 2 [1:-th] 
whenBlock -th 
#### Usage

`Usage:`    whenBlock [-a|-l|-h] date/block  
`Purpose:`  Finds the nearest block before the JSON-formatted date, or the date of the given block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date/block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber |
| -a | --alone | show the found block or found date unadorned (useful for scripting) |
| -l | --list | list special blocks timestamps and dates |
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
