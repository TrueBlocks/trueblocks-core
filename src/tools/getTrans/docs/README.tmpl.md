## [{NAME}]

The `getTrans` tool retrieves transactions from the running Ethereum node (using the `--raw` option) or from QuickBlocks (the default). You may provide a transaction `hash`, a `blockNumber.transactionID` pair, or a `blockHash.transactionID` pair (or any combination) to specify the transaction(s).

Use the `--trace` option to display a trace of the transaction as well.

[{USAGE_TABLE}]*See Also*: This command-line tool implements these RPC interfaces:
[eth_getTransactionByHash](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyhash), 
[eth_getTransactionByBlockNumberAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblocknumberandindex),
[eth_getTransactionByBlockHashAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblockhashandindex).

[{FOOTER}]
