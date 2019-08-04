## getTrans

The `getTrans` tool retrieves transactions from the running Ethereum node (using the `--raw` option) or from QBlocks (the default). You may provide a transaction `hash`, a `blockNumber.transactionID` pair, or a `blockHash.transactionID` pair (or any combination) to specify the transaction(s).

#### Usage

`Usage:`    getTrans [-a|-t|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve an Ethereum transaction from the local cache or a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | trans_list | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) (required) |
| -a | --articulate | articulate the transactions if an ABI is found for the 'to' address |
| -t | --trace | display the transaction's trace |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `trans_list` is one or more space-separated identifiers which may be either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results may be undefined.

*See Also*: This command-line tool implements these RPC interfaces:
[eth_getTransactionByHash](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_gettransactionbyhash),
[eth_getTransactionByBlockNumberAndIndex](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_gettransactionbyblocknumberandindex),
[eth_getTransactionByBlockHashAndIndex](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_gettransactionbyblockhashandindex).

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --veryRaw   |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --api_mode  |   simulate api_mode for testing
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


