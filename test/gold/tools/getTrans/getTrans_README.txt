getTrans argc: 2 [1:-th] 
getTrans -th 
#### Usage

`Usage:`    getTrans [-t|-a|-v|-h] &lt;transID&gt; [transID...]  
`Purpose:`  Retrieve an Ethereum transaction from the local cache or a running node.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | trans_list | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) |
| -t | --trace | display the transaction's trace |
| -a | --articulate | articulate the transactions if an ABI is found |

#### Hidden options (shown during testing only)
| -b | --belongs val | report true or false if the given address is found anywhere in the transaction |
| -a | --asStrs | when checking --belongs, treat input and log data as a string |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `trans_list` is one or more space-separated identifiers which may be either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results may be undefined.

