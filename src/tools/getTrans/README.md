## getTrans

The `getTrans` tool retrieves transactions from the running Ethereum node (using the `--fromNode` option) or from QuickBlocks (the default). You may provide a transaction `hash`, a `blockNumber.transactionID` pair, or a `blockHash.transactionID` pair (or any combination) to specify the transaction(s).

Use the `--trace` option to display a trace of the transaction as well.

*See Also*: This command-line tool implements these RPC interfaces:
[eth_getTransactionByHash](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyhash), 
[eth_getTransactionByBlockNumberAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblocknumberandindex),
[eth_getTransactionByBlockHashAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblockhashandindex).
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
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

