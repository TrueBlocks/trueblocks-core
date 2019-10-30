acctExport argc: 2 [1:-th] 
acctExport -th 
#### Usage

`Usage:`    acctExport [-p|-l|-t|-b|-e|-c|-a|-w|-x|-r|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses (0x...) to export (required) |
| -p | --appearances | export a list of appearances |
| -l | --logs | export logs instead of transaction list |
| -t | --traces | export traces instead of transaction list |
| -b | --balances | export balance history instead of transaction list |
| -e | --hashes_only | export the IPFS hashes of the index chunks the address appears in |
| -c | --count_only | display only the count of the number of data items requested |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -w | --write_blocks | toggle writing blocks to the binary cache ('off' by default) |
| -x | --write_txs | toggle writing transactions to the cache ('on' by default) |
| -r | --write_traces | toggle writing traces to the cache ('on' by default) |

#### Hidden options (shown during testing only)
| -s | --skip_ddos | toggle skipping over 2016 dDos transactions ('on' by default) |
| -m | --max_traces <num> | if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) |
| -A | --all_abis | load all previously cached abi files |
| -g | --grab_abis | using each trace's 'to' address, grab the abi for that address (improves articulation) |
| -f | --freshen | freshen but do not print the exported data |
| -D | --deltas | for --balances option only, export only changes in balances |
| -S | --start <num> | first block to process (inclusive) |
| -E | --end <num> | last block to process (inclusive) |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

