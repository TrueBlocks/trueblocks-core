ethprice argc: 2 [1:-th] 
ethprice -th 
#### Usage

`Usage:`    ethprice -a|-f|-p|-w|-t|-v|-h  
`Purpose:`  Freshen and/or display price data for the Ethereum network and other purposes.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -a | --at | Report the price since to the nearest five minutes to :ts (timestamp) |
| -f | --freshen | Freshen database (append new data) |
| -p | --period | Display prices in this increment. Default 120 mins or :t where t is in [5&#124;15&#124;30&#124;120&#124;240&#124;1440] |
| -w | --when | Time of day to start the display. :h determines the hour to start |
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

#### Powered by QuickBlocks&reg;
