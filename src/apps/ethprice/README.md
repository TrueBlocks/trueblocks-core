## ethprice

The `ethprice` tool provides Ethereum price data to various **quickBlocks** tools or for other purposes. Currently, `ethprice` retrieves data using the Poloniex API. In future versions, we intend to add other sources of pricing data and provide a mechanism to specify an averaging calculate given multiple price sources. Ultimately, we hope Ethereum/fiat price data appears under consensus, but until that time, **quickBlocks** uses `ethprice`. Powered, in part, by Poloniex<sup>&reg;<sup>
#### Usage

`Usage:`    ethprice -a|-c|-f|-p|-v|-h  
`Purpose:`  Freshen and/or display Ethereum price data and other purposes.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -a | --at val | Report the price since nearest five minutes to the given timestamp |
| -c | --current | Report on the current price (i.e. -at:now) |
| -f | --freshen | Freshen database (append new data) |
| -p | --period val | Display prices in this increment. One of [5&#124;15&#124;30&#124;120*&#124;240&#124;1440] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


