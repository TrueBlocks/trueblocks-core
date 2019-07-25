getTrace argc: 2 [1:-th] 
getTrace -th 
#### Usage

`Usage:`    getTrace [-a|-c|-n|-v|-h] &lt;transID&gt; [transID...]  
`Purpose:`  Retrieve a transaction's traces from the local cache or a running node.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | trans_list | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) |
| -a | --articulate | articulate the transactions if an ABI is found for the 'to' address |
| -c | --countOnly | show the number of traces for the transaction only (fast) |
| -n | --noHeader | do not show the header row |

#### Hidden options (shown during testing only)
| -x | --fmt val | export format (one of [none&#124;json&#124;txt&#124;csv&#124;api]) |
| -d | --ddos on/off | skip over dDos transactions in export ('on' by default) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `trans_list` is one or more space-separated identifiers which may be either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results may be undefined.

