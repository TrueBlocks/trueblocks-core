ethQuote argc: 2 [1:-th] 
ethQuote -th 
#### Usage

`Usage:`    ethQuote [-f|-p|-a|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -f | --freshen | Freshen price database (append new data) |
| -p | --period <val> | display prices in this increment, one of [5&#124;15&#124;30&#124;60&#124;120*&#124;240&#124;1440] |
| -a | --pair <str> | which price pair to freshen or list (see Poloniex) |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here:
  https://poloniex.com/public?command=returnCurrencies.
- `Note`: Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

