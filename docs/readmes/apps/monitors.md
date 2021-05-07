## chifra monitors

This folder contains a TrueBlocks monitor. TrueBlocks monitors pull transactions from the Ethereum blockchain for a given (or a series of) Ethereum addresses.

Below we present the command line interface to this tool, although the tool itself is not available under open source. While the tool is in active development, TrueBlocks monitors already produce very useful results. For example, we use TrueBlocks monitors to account for and analyze all transactions on a given smart contract. We present [this example](http://dao.quickblocks.io).

Please contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.

### usage

`Usage:`    chifra export [-p|-r|-l|-t|-C|-O|-a|-i|-R|-U|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses (0x...) to export (required) |
|  | topics | filter by one or more logs topics (only for --logs option) |
|  | fourbytes | filter by one or more fourbytes (only for transactions and trace options) |
| -p | --appearances | export a list of appearances |
| -r | --receipts | export receipts instead of transaction list |
| -l | --logs | export logs instead of transaction list |
| -t | --traces | export traces instead of transaction list |
| -C | --accounting | export accounting records instead of transaction list |
| -O | --tokens | export accounting for ERC 20 tokens (assumes ETH accounting as above) |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -i | --cache_txs | write transactions to the cache (see notes) |
| -R | --cache_traces | write traces to the cache (see notes) |
| -U | --count | only available for --appearances mode, if present return only the number of records |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

