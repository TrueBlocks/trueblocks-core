acctExport argc: 2 [1:--readme] 
acctExport --readme 
### Usage

`Usage:`    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more addresses.

`Where:`

{{<td>}}
|          | Option                                 | Description                                                                                                                                           |
| -------- | -------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
|          | addrs                                  | one or more addresses (0x...) to export (required)                                                                                                    |
|          | topics                                 | filter by one or more log topics (only for --logs option)                                                                                             |
|          | fourbytes                              | filter by one or more fourbytes (only for transactions<br/>and trace options)                                                                         |
| &#8208;p | &#8208;&#8208;appearances              | export a list of appearances                                                                                                                          |
| &#8208;r | &#8208;&#8208;receipts                 | export receipts instead of transaction list                                                                                                           |
| &#8208;A | &#8208;&#8208;statements               | for use with --accounting option only, export only<br/>reconciliation statements                                                                      |
| &#8208;l | &#8208;&#8208;logs                     | export logs instead of transaction list                                                                                                               |
| &#8208;t | &#8208;&#8208;traces                   | export traces instead of transaction list                                                                                                             |
| &#8208;C | &#8208;&#8208;accounting               | export accounting records instead of transaction list                                                                                                 |
| &#8208;a | &#8208;&#8208;articulate               | articulate transactions, traces, logs, and outputs                                                                                                    |
| &#8208;i | &#8208;&#8208;cache_txs                | write transactions to the cache (see notes)                                                                                                           |
| &#8208;R | &#8208;&#8208;cache_traces             | write traces to the cache (see notes)                                                                                                                 |
| &#8208;y | &#8208;&#8208;factory                  | scan for contract creations from the given address(es)<br/>and report address of those contracts                                                      |
|          | &#8208;&#8208;emitter                  | for log export only, export only if one of the given<br/>export addresses emitted the event                                                           |
|          | &#8208;&#8208;source string            | for log export only, export only one of these addresses<br/>emitted the event                                                                         |
|          | &#8208;&#8208;relevant                 | for log and accounting export only, if true export<br/>only logs relevant to one of the given export addresses                                        |
| &#8208;U | &#8208;&#8208;count                    | only available for --appearances mode, if present return<br/>only the number of records                                                               |
| &#8208;c | &#8208;&#8208;first_record &lt;num&gt; | the first record to process                                                                                                                           |
| &#8208;e | &#8208;&#8208;max_records &lt;num&gt;  | the maximum number of records to process before reporting                                                                                             |
|          | &#8208;&#8208;clean                    | clean (i.e. remove duplicate appearances) from all<br/>existing monitors                                                                              |
| ###      | Hidden options                         |                                                                                                                                                       |
| &#8208;f | &#8208;&#8208;freshen                  | freshen but do not print the exported data                                                                                                            |
| &#8208;s | &#8208;&#8208;staging                  | enable search of staging (not yet finalized) folder                                                                                                   |
| &#8208;u | &#8208;&#8208;unripe                   | enable search of unripe (neither staged nor finalized)<br/>folder (assumes --staging)                                                                 |
|          | &#8208;&#8208;load string              | a comma separated list of dynamic traversers to load                                                                                                  |
|          | &#8208;&#8208;reversed                 | produce results in reverse chronological order                                                                                                        |
| &#8208;b | &#8208;&#8208;by_date                  | produce results sorted by date (default is to report<br/>by address)                                                                                  |
| &#8208;z | &#8208;&#8208;summarize_by &lt;val&gt; | for --accounting only, summarize reconciliations by<br/>this time period, one of [yearly, quarterly, monthly,<br/>weekly, daily, hourly, blockly, tx] |
| &#8208;F | &#8208;&#8208;first_block &lt;num&gt;  | first block to process (inclusive)                                                                                                                    |
| &#8208;L | &#8208;&#8208;last_block &lt;num&gt;   | last block to process (inclusive)                                                                                                                     |
| ###      | Hidden options                         |                                                                                                                                                       |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;          | export format, one of [none, json, txt, csv, api]                                                                                                     |
| &#8208;v | &#8208;&#8208;verbose                  | set verbose level (optional level defaults to 1)                                                                                                      |
| &#8208;h | &#8208;&#8208;help                     | display this help screen                                                                                                                              |
{{</td>}}

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

`Configurable Items:`

- `cache_txs`: write transactions to the cache (see notes).
- `cache_traces`: write traces to the cache (see notes).
- `skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).
- `max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction
  is (default = 250).

