## chifra transactions

The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw` option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attachs an array transaction traces to the output (if the node you're querying has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances instead of the underlying data (including uniq addresses in traces if enabled).

### usage

`Usage:`    chifra transactions [-a|-t|-u|-r|-o|-v|-h] &lt;tx_id&gt; [tx_id...]
`Purpose:`  Retrieve one or more transactions from the chain or local cache.

`Where:`

{{<td>}}
|     | Option                      | Description                                                                                  |
| --- | --------------------------- | -------------------------------------------------------------------------------------------- |
|     | transactions                | a space-separated list of one or more transaction identifiers<br/>(required)                 |
| -a  | --articulate                | articulate the retrieved data if ABIs can be found                                           |
| -t  | --trace                     | include the transaction's traces in the results                                              |
| -u  | --uniq                      | display a list of uniq addresses found in the transaction<br/>instead of the underlying data |
| -r  | --reconcile &lt;address&gt; | reconcile the transaction as per the provided address                                        |
| -o  | --cache                     | force the results of the query into the tx cache (and<br/>the trace cache if applicable)     |
| -x  | --fmt &lt;val&gt;           | export format, one of [none, json, txt, csv, api]                                            |
| -v  | --verbose                   | set verbose level (optional level defaults to 1)                                             |
| -h  | --help                      | display this help screen                                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

**Source code**: [`tools/getTrans`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTrans)

