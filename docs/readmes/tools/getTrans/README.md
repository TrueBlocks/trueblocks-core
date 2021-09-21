## chifra transactions

The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw` option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attachs an array transaction traces to the output (if the node you're querying has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances instead of the underlying data (including uniq addresses in traces if enabled).

### usage

`Usage:`    chifra transactions [-a|-t|-u|-r|-o|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve one or more transactions from the chain or local cache.

`Where:`

{{<td>}}
|          | Option                                  | Description                                                                                  |
| -------- | --------------------------------------- | -------------------------------------------------------------------------------------------- |
|          | transactions                            | a space-separated list of one or more transaction identifiers<br/>(required)                 |
| &#8208;a | &#8208;&#8208;articulate                | articulate the retrieved data if ABIs can be found                                           |
| &#8208;t | &#8208;&#8208;trace                     | include the transaction's traces in the results                                              |
| &#8208;u | &#8208;&#8208;uniq                      | display a list of uniq addresses found in the transaction<br/>instead of the underlying data |
| &#8208;r | &#8208;&#8208;reconcile &lt;address&gt; | reconcile the transaction as per the provided address                                        |
| &#8208;o | &#8208;&#8208;cache                     | force the results of the query into the tx cache (and<br/>the trace cache if applicable)     |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;           | export format, one of [none, json, txt, csv, api]                                            |
| &#8208;v | &#8208;&#8208;verbose                   | set verbose level (optional level defaults to 1)                                             |
| &#8208;h | &#8208;&#8208;help                      | display this help screen                                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

**Source code**: [`tools/getTrans`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTrans)

