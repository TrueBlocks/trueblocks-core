## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated string of the same values used by `trace_fitler`.

### usage

`Usage:`    chifra traces [-a|-f|-d|-c|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve traces for the given transaction(s).

`Where:`

{{<td>}}
|     | Option               | Description                                                                  |
| --- | -------------------- | ---------------------------------------------------------------------------- |
|     | transactions         | a space-separated list of one or more transaction identifiers<br/>(required) |
| -a  | --articulate         | articulate the retrieved data if ABIs can be found                           |
| -f  | --filter &lt;str&gt; | call the node's `trace_filter` routine with bang-seperated<br/>filter        |
| -d  | --statediff          | export state diff traces (not implemented)                                   |
| -c  | --count              | show the number of traces for the transaction only<br/>(fast)                |
| -x  | --fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                            |
| -v  | --verbose            | set verbose level (optional level defaults to 1)                             |
| -h  | --help               | display this help screen                                                     |
{{</td>}}

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.
- A bang seperated filter has the following fields (at least one of which is required) and is separated
  with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
- A state diff trace describes, for each modified address, what changed during that trace.

`Configurable Items:`

`skip_ddos`: skip over the 2016 ddos during export ('on' by default).
`max`: if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250).

**Source code**: [`tools/getTraces`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTraces)

