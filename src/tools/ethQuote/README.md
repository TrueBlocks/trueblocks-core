## ethQuote

The `ethQuote` tool provides Ethereum price data to various tools or for other purposes. Currently, `ethQuote` retrieves data using the Poloniex API. In future versions, we intend to add other sources of pricing data and provide a mechanism to specify an averaging calculate given multiple price sources. Ultimately, we hope Ethereum/fiat price data appears under consensus, but until that time, **QBlocks** uses `ethQuote`. Powered, in part, by Poloniex<sup>&reg;<sup>
#### Usage

`Usage:`    ethQuote -a|-c|-d|-f|-p|-p|-v|-h  
`Purpose:`  Freshen and/or display Ethereum price data and other purposes.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -a | --at <ts> | Report the price since nearest five minutes to the given timestamp |
| -c | --current | Report on the current price (i.e. -at:now) |
| -d | --data | Export prices as JSON data |
| -f | --freshen | Freshen database (append new data) |
| -p | --period enum<5|15|30|*120|240|1440> | Display prices in this increment. One of [5&#124;15&#124;30&#124;120*&#124;240&#124;1440] |
| -p | --pair <val> | Which price pair to freshen or list (see Poloniex) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here:
  https://poloniex.com/public?command=returnCurrencies.
- `Note`: Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --veryRaw   |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


