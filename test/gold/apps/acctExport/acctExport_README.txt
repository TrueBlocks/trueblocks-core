acctExport argc: 2 [1:-th] 
acctExport -th 
### Usage

`Usage:`    chifra export [-p|-r|-l|-t|-C|-O|-a|-i|-R|-U|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses (0x...) to export (required) |
|  | topics | filter by one or more logs topics (only for --logs option) |
|  | fourbytes | filter by one or more fourbytes (only for transactions and trace options) |
| -p | --appearances | export a list of appearances |
| -r | --receipts | export receipts instead of transaction list |
| -l | --logs | export logs instead of transaction list |
| -t | --traces | export traces instead of transaction list |
| -C | --accounting | export accounting records instead of transaction list |
| -O | --tokens | export accounting for ERC 20 tokens (assumes ETH accounting as above) |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -i | --cache_txs | write transactions to the cache (see notes) |
| -R | --cache_traces | write traces to the cache (see notes) |
| -U | --count | only available for --appearances mode, if present return only the number of records |
|####|Hidden options||
| -d | --skip_ddos | toggle skipping over 2016 dDos transactions ('on' by default) |
| -m | --max_traces <num> | if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) |
| -f | --freshen | freshen but do not print the exported data |
| -y | --factory | scan for contract creations from the given address(es) and report address of those contracts |
|  | --emitter | for log export only, export only if one of the given export addresses emitted the event |
|  | --emitted_by list<addr> | for log export only, export only one of these addresses emitted the event |
|  | --relevant | for log export only, if true export only logs relevant to one of the given export addresses |
| -c | --first_record <num> | the first record to process |
| -e | --max_records <num> | the maximum number of records to process before reporting |
|  | --clean | clean (i.e. remove duplicate appearances) from all existing monitors |
| -s | --staging | enable search of staging (not yet finalized) folder |
| -u | --unripe | enable search of unripe (neither staged nor finalized) folder (assumes --staging) |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

