## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

### Usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
| -f | --freshen | Freshen price database (append new data) |
| -p | --period <val> | increment of display, one of *[ 5 \| 15 \| 30 \| 60 \| 120\* \| 240 \| 1440 \| 10080 \| hourly \| daily \| weekly ]* |
| -a | --pair <str> | which price pair to freshen or list (see Poloniex) |
| -e | --feed <val> | the feed for the price data, one of *[ poloniex\* \| maker \| tellor ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here: 
  https://poloniex.com/public?command=returnCurrencies.
- Due to restrictions from Poloniex, this tool retrieves only 30 days of data 
  at a time. You must repeatedly run this command until the data is up-to-date.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/getQuotes`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getQuotes)

