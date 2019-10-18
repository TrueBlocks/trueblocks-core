ethQuote argc: 2 [1:-th] 
ethQuote -th 
#### Usage

`Usage:`    ethQuote -f|-p|-r|-v|-h  
`Purpose:`  Freshen and/or display Ethereum price data and other purposes.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -f | --freshen | Freshen database (append new data) |
| -p | --period <val> | Display prices in this increment, one of [5&#124;15&#124;30&#124;120*&#124;240&#124;1440] |
| -r | --pair <pair> | Which price pair to freshen or list (see Poloniex) |

#### Hidden options (shown during testing only)
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here:
  https://poloniex.com/public?command=returnCurrencies.
- `Note`: Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

