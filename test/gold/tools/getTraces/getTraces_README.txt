getTraces argc: 2 [1:--readme] 
getTraces --readme 
### Usage

`Usage:`    chifra traces [-a|-f|-d|-c|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve traces for the given transaction(s).

`Where:`

{{<td>}}
|          | Option                        | Description                                                                                   |
| -------- | ----------------------------- | --------------------------------------------------------------------------------------------- |
|          | transactions                  | a space-separated list of one or more transaction identifiers<br/>(required)                  |
| &#8208;a | &#8208;&#8208;articulate      | articulate the retrieved data if ABIs can be found                                            |
| &#8208;f | &#8208;&#8208;filter string   | call the node's trace_filter routine with bang-seperated<br/>filter                           |
| &#8208;d | &#8208;&#8208;statediff       | export state diff traces (not implemented)                                                    |
| &#8208;c | &#8208;&#8208;count           | show the number of traces for the transaction only<br/>(fast)                                 |
| ###      | Hidden options                |                                                                                               |
| &#8208;s | &#8208;&#8208;skip_ddos       | skip over the 2016 ddos during export ('on' by default)                                       |
| &#8208;m | &#8208;&#8208;max &lt;num&gt; | if --skip_ddos is on, this many traces defines what<br/>a ddos transaction is (default = 250) |
| ###      | Hidden options                |                                                                                               |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                             |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                              |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                                      |
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

- `skip_ddos`: skip over the 2016 ddos during export ('on' by default).
- `max`: if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250).

