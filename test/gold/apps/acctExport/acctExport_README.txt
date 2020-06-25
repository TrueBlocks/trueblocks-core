acctExport argc: 2 [1:-th] 
acctExport -th 
#### Usage

`Usage:`    acctExport [-p|-r|-l|-t|-b|-C|-a|-i|-R|-M|-U|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses (0x...) to export (required) |
| -p | --appearances | export a list of appearances |
| -r | --receipts | export receipts instead of transaction list |
| -l | --logs | export logs instead of transaction list |
| -t | --traces | export traces instead of transaction list |
| -b | --balances | export balance history instead of transaction list |
| -C | --accounting | export accounting records instead of transaction list |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -i | --write_txs | write transactions to the cache (see notes) |
| -R | --write_traces | write traces to the cache (see notes) |
| -M | --emitter | available for --logs option only, export will only export if the address emitted the event |
| -U | --count | only available for --appearances mode, if present return only the number of records |

#### Hidden options (shown during testing only)
| -s | --skip_ddos | toggle skipping over 2016 dDos transactions ('on' by default) |
| -m | --max_traces <num> | if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) |
| -A | --all_abis | load all previously cached abi files |
| -f | --freshen | freshen but do not print the exported data |
| -F | --freshen_max <num> | maximum number of records to process for --freshen option |
| -D | --deltas | for --balances option only, export only changes in balances |
| -c | --first_record <num> | the first record to process |
| -e | --max_records <num> | the maximum number of records to process before reporting |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- By default, transactions and traces are cached if the number of exported 
  items is <= to 1,000 items. Otherwise, if you specify any `write_*` options, 
  your preference predominates.

