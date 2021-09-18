## chifra receipts

`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using `chifra transactions` and more focused data using `chifra logs`. It is included here for completeness, as the `receipt` is a fundamental data structure in Ethereum.

### usage

`Usage:`    chifra receipts [-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve receipts for the given transaction(s).

`Where:`

{{<td>}}
|          | Option                        | Description                                                                  |
| -------- | ----------------------------- | ---------------------------------------------------------------------------- |
|          | transactions                  | a space-separated list of one or more transaction identifiers<br/>(required) |
| &#8208;a | &#8208;&#8208;articulate      | articulate the retrieved data if ABIs can be found                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                            |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                             |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

**Source code**: [`tools/getReceipts`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getReceipts)

