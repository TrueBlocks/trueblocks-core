getReceipts argc: 2 [1:-th] 
getReceipts -th 
### Usage

`Usage:`    chifra receipts [-a|-l|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve a transaction's receipt from the cache or the node.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | transactions | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) (required) |
| -a | --articulate | articulate the transactions if an ABI is found for the 'to' address |
| -l | --logs | display the receipt's logs |
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `transactions` is one or more space-separated identifiers which may be either a transaction hash, 
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- If the queried node does not store historical state, the results may be undefined.

