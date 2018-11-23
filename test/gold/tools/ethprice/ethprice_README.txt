ethprice argc: 2 [1:-th] 
ethprice -th 
#### Usage

`Usage:`    ethprice -a|-c|-f|-p|-p|-v|-h  
`Purpose:`  Freshen and/or display Ethereum price data and other purposes.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -a | --at val | Report the price since nearest five minutes to the given timestamp |
| -c | --current | Report on the current price (i.e. -at:now) |
| -f | --freshen | Freshen database (append new data) |
| -p | --period val | Display prices in this increment. One of [5&#124;15&#124;30&#124;120*&#124;240&#124;1440] |
| -p | --pair val | Which price pair to freshen or list (see Poloniex) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

