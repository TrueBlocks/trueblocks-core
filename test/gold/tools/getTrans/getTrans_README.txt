getTrans argc: 2 [1:-th] 
getTrans -th 
#### Usage

`Usage:`    getTrans [-f|-t|-v|-h] hash bn.transID bh.transID  
`Purpose:`  Retrieve an Ethereum transaction from either QuickBlocks or a running node.
            --note: 'hash' and 'blockHash' must start with '0x'.  
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | hash | one or more hashes of Ethereum transactions, or |
|  | bn.transID | blockNumber.transactionID of one or more Ethereum transactions, or |
|  | bh.transID | blockHash.transactionID of one or more Ethereum transactions |
| -f | --fromNode | retrieve the transaction from the running node (from QuickBlocks otherwise) |
| -t | --trace | return the trace of the transaction in addition to regular details |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

