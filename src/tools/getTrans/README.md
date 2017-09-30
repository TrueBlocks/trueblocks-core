## getTrans

The `getTrans` tool retrieves transactions from the running Ethereum node (using the `--fromNode` option) or from QuickBlocks (the default). You may provide a transaction `hash`, a `blockNumber.transactionID` pair, or a `blockHash.transactionID` pair (or any combination) to specify the transaction(s).

Use the `--trace` option to display a trace of the transaction as well.

#### Usage

`Usage:`    getTrans [-f|-t|-v|-h] &lt;hash|bn.transID|bh.transID&gt;  
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

*See Also*: This command-line tool implements these RPC interfaces:
[eth_getTransactionByHash](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyhash), 
[eth_getTransactionByBlockNumberAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblocknumberandindex),
[eth_getTransactionByBlockHashAndIndex](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_gettransactionbyblockhashandindex).

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


