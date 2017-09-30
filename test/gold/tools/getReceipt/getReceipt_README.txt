getReceipt argc: 2 [1:-th] 
getReceipt -th 
#### Usage

`Usage:`    getReceipt [-f|-a|-v|-h] &lt; txhash | bn.transID | bh.transID | address &gt;
  
`Purpose:`  Retrieve a transaction's receipt using either QuickBlocks or a running node.
            --note: 'hash' and 'blockHash' must start with '0x'.  
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | txhash | one or more hashes of Ethereum transactions, or |
|  | bn.transID | blockNumber.transactionID of one or more Ethereum transactions, or |
|  | bh.transID | blockHash.transactionID of one or more Ethereum transactions, or |
|  | address | if --address, then an Ethereum address |
| -f | --fromNode | retrieve the transaction from the running node (from QuickBlocks otherwise) |
| -a | --address | retrieve all logs (from node) given a list of one or more Ethereum addresses |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

