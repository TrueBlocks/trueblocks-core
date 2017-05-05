## getTrans

The `getTrans` tool retrieves transactions from the running Ethereum node (if you specify the `--fromNode` option) or from QuickBlocks (the default). You may provide either a transaction `hash` or a `blockNumber.transactionID` pair to specify the transaction. Use the `--trace` option to display a trace of the transaction as well.

#### Help Screen

`Usage:`    getTrans [-f|-t|-v|-h] hash or bn.transID  
`Purpose:`  Retrieve an Ethereum transaction from its hash.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | hash or | hash of an Ethereum transaction, or |
|  | bn.transID | blockNumber and transactionID of an Ethereum transaction |
| -f | --fromNode | retrieve the transaction from the running node (from QuickBlocks otherwise) |
| -t | --trace | return the trace of the transaction in addition to regular details |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Please a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
