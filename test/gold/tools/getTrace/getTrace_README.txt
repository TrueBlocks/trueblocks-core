getTrace argc: 2 [1:-th] 
getTrace -th 
#### Usage

`Usage:`    getTrace [-a|-c|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve a transaction's traces from the local cache or a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | transactions | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) (required) |
| -a | --articulate | articulate the transactions if an ABI is found for the 'to' address |
| -c | --count_only | show the number of traces for the transaction only (fast) |

#### Hidden options (shown during testing only)
| -s | --skip_ddos | toggle skipping over 2018 ddos transactions during export ('on' by default) |
| -m | --max_traces <num> | if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) |
| -f | --filter <str> | Call trace_filter with the comma seperated string of the filter (see docs) |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `transactions` is one or more space-separated identifiers which may be either a transaction hash, 
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- If the queried node does not store historical state, the results are undefined.

