acctExport argc: 2 [1:-th] 
acctExport -th 
#### Usage

`Usage:`    acctExport [-x|-a|-l|-t|-c|-p|-o|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses (0x...) to export (required) |
| -x | --fmt <fmt> | export format (one of [json*&#124;txt&#124;csv]) |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -l | --logs | export logs instead of transaction list |
| -t | --traces | export traces instead of transaction list |
| -c | --balances | export balance history instead of transaction list |
| -p | --appearances | export a list of appearances |
| -o | --count_only | display only the count of the number of data items requested |

#### Hidden options (shown during testing only)
| -b | --blocks enum[on|off*] | write blocks to the binary cache ('off' by default) |
| -s | --writeTxs enum[on*|off] | write transactions to the binary cache ('on' by default) |
| -r | --writeTraces enum[on*|off] | write traces to the binary cache ('on' by default) |
| -d | --ddos enum[on*|off] | skip over dDos transactions in export ('on' by default) |
| -m | --maxTraces <uint> | if --ddos:on, the number of traces defining a dDos (default = 250) |
| -n | --noHeader | do not show the header row |
| -a | --allABIs | load all previously cached abi files |
| -g | --grabABIs | using each trace's 'to' address, grab the abi for that address (improves articulation) |
| -f | --freshen | freshen but do not print the exported data |
| -s | --start <num> | first block to export (inclusive) |
| -e | --end <num> | last block to export (inclusive) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

