## ethprice

The `ethprice` tool provides Ethereum price data to various `quickBlocks` tools or for other purposes. Currently, the `ethprice` retrieves data using the Poloniex API. In future versions, we intend to add other sources of pricing data and provide a mechanism to specify an averaging calculate given multiple price sources. Ultimately, we hope Ethereum/fiat price data appears under consensus, but until that time, QuickBlocks uses `ethprice`. Powered, in part, by Poloniex<sup>&reg;<sup>
#### Usage

`Usage:`    ethprice -a|-f|-p|-w|-t|-v|-h  
`Purpose:`  Freshen and/or display Ethereum price data and other purposes.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -a | --at val | Report the price since nearest five mintes to the given timestamp |
| -f | --freshen | Freshen database (append new data) |
| -p | --period val | Display prices in this increment. One of [5&#124;15&#124;30&#124;120*&#124;240&#124;1440] |
| -w | --when val | Hour on which to start display. Integer between 0-23 |
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


