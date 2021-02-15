acctScrape argc: 2 [1:-th] 
acctScrape -th 
NO_NAMES = [true]
#### Usage

`Usage:`    acctScrape [-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Add or remove monitors for a given Ethereum address (or collection of addresses).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more Ethereum addresses (required) |
|####|Hidden options||
| -s | --staging | enable search of staging (not yet finalized) folder |
| -u | --unripe | enable search of unripe (neither staged nor finalized) folder (requires --staging) |
| -c | --clean | clean (i.e. remove dups) from all existing monitors |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

