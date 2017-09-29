getLogs argc: 2 [1:-th] 
getLogs -th 
#### Usage

`Usage:`    getLogs [-f|-a|-v|-h] &lt; hash | bn.transID | bh.transID | address &gt;
            -- note: This tool is incomplete.
  
`Purpose:`  Retrieve logs from an Ethereum transaction using either QuickBlocks or a running node.
            --note: 'hash' and 'blockHash' must start with '0x'.  
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | hash | one or more hashes of Ethereum transactions, or |
|  | bn.transID | blockNumber.transactionID of one or more Ethereum transactions, or |
|  | bh.transID | blockHash.transactionID of one or more Ethereum transactions, or |
|  | address | if --address, then an Ethereum address |
| -f | --fromNode | retrieve the transaction from the running node (from QuickBlocks otherwise) |
| -a | --address | retrieve all logs (from node) given a list of one or more Ethereum addresses |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

