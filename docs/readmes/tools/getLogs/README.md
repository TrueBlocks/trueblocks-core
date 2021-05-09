## chifra logs

`chifra logs` returns a transaction's logs. You may specify the logs you want using either a transaction hash (trans\_hash), a block hash plus transaction index (block\_hash.trans\_id), or a block number plus transaction index (block\_num.trans\_id).  

### usage

`Usage:`    chifra logs [-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve a transaction's logs from the cache or the node.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | transactions | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) (required) |
| -a | --articulate | articulate the transactions if an ABI is found for the 'to' address |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- `transactions` is one or more space-separated identifiers which may be either a transaction hash, 
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- If the queried node does not store historical state, the results may be undefined.

**Source code**: [`tools/getLogs`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getLogs)

