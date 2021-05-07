getQuotes argc: 2 [1:-th] 
getQuotes -th 
### Usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
| -f | --freshen | Freshen price database (append new data) |
| -p | --period <val> | increment of display, one of [5&#124;15&#124;30&#124;60&#124;120*&#124;240&#124;1440&#124;10080&#124;hourly&#124;daily&#124;weekly] |
| -a | --pair <str> | which price pair to freshen or list (see Poloniex) |
| -e | --feed <val> | the feed for the price data, one of [poloniex*&#124;maker&#124;tellor] |
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here: 
  https://poloniex.com/public?command=returnCurrencies.
- `Note`: Due to restrictions from Poloniex, this tool retrieves only 30 days of data 
  at a time. You must repeatedly run this command until the data is up-to-date.

