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
| -s | --staging | produce results in the staging folder instead of production folder |
| -u | --unripe | visit unripe (not old enough and not yet staged or finalized) blocks |
| -d | --daemon | we are being called in daemon mode which causes us to print results differently |
| -S | --start <num> | first block to process(inclusive) |
| -E | --end <num> | last block to process (inclusive) |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

                                                                                                                        