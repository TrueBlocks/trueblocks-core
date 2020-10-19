acctScrape argc: 2 [1:-th] 
acctScrape -th 
#### Usage

`Usage:`    acctScrape [-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Add or remove monitors for a given Ethereum address (or collection of addresses).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more Ethereum addresses (required) |

#### Hidden options (shown during testing only)
| -s | --staging | enable search of staging (not yet finalized) folder |
| -u | --unripe | enable search of unripe (neither staged nor finalized) folder (requires --staging) |
| -b | --blooms | process query by first using bloom filter and, if hit, downloading index chunk from remote |
| -S | --start <num> | this value is ignored but remains for backward compatibility |
| -E | --end <num> | this value is ignored but remains for backward compatibility |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

