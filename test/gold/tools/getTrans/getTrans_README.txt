getTrans argc: 2 [1:-th] 
getTrans -th 
#### Usage

`Usage:`    getTrans [-r|-v|-h] &lt;transID&gt; [transID...]  
`Purpose:`  Retrieve an Ethereum transaction from the local cache or a running node.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | trans_list | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) |
| -r | --raw | retrieve raw transaction directly from the running node |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `trans_list` is one or more space-separated identifiers which may be either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- If the queried node does not store historical state, the results may be undefined.

