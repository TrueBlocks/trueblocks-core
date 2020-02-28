acctScrape argc: 2 [1:-th] 
acctScrape -th 
#### Usage

`Usage:`    acctScrape [-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Index transactions for a given Ethereum address (or series of addresses).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more Ethereum addresses (required) |

#### Hidden options (shown during testing only)
| -f | --finalized | toggle search of finalized folder ('on' by default) |
| -s | --staging | toggle search of staging (not yet finalized) folder ('off' by default) |
| -u | --unripe | toggle search of unripe (neither staged nor finalized) folder ('off' by default) |
| -S | --start <num> | first block to process (inclusive) |
| -E | --end <num> | last block to process (inclusive) |
| -i | --silent | lighten the reporting on progress (for use with --daemon switch to `chifra scrape`) |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

