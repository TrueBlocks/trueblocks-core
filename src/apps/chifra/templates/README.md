
`Usage:`    bin/[{PROGNAME}] [-f|-c|-s|-l|-a|-n|-r|-v|-h]  
`Purpose:`  Index transactions for a given Ethereum address (or series of addresses).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -f | --freshenOnly | freshen only -- default (do not display transactions from the cache) |
| -c | --cacheOnly | display transactions from the cache only (do not freshen) |
| -s | --showCache | show transactions from the cache, and then freshen |
| -l | --logs | display smart contract events (logs) |
| -a | --accounting | display credits and debits and reconcilation per block |
| -r | --rebuild | clear cache and reprocess all transcations (may take a long time) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

