getTrans argc: 2 [1:-th] 
getTrans -th 
### Usage

`Usage:`    chifra transactions [-a|-t|-u|-o|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieves one or more transactions from the cache or the node.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | transactions | a space-separated list of one or more transaction identifiers (required) |
| -a | --articulate | articulate the retrieved data if ABIs can be found |
| -t | --trace | include the transaction's traces in the results |
| -u | --uniq | display a list of uniq addresses found in the transaction instead of the underlying data |
| -o | --cache | force the results of the query into the tx cache (and the trace cache if applicable) |
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash, 
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

