## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

### usage

`Usage:`    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]
`Purpose:`  Export full detail of transactions for one or more addresses.

`Where:`

{{<td>}}
|     | Option                     | Description                                                                                                    |
| --- | -------------------------- | -------------------------------------------------------------------------------------------------------------- |
|     | addrs                      | one or more addresses (0x...) to export (required)                                                             |
|     | topics                     | filter by one or more log topics (only for --logs option)                                                      |
|     | fourbytes                  | filter by one or more fourbytes (only for transactions<br/>and trace opt)                                      |
| -p  | --appearances              | export a list of appearances                                                                                   |
| -r  | --receipts                 | export receipts instead of transaction list                                                                    |
| -A  | --statements               | for use with --accounting option only, export only<br/>reconciliation statements                               |
| -l  | --logs                     | export logs instead of transaction list                                                                        |
| -t  | --traces                   | export traces instead of transaction list                                                                      |
| -C  | --accounting               | export accounting records instead of transaction list                                                          |
| -a  | --articulate               | articulate transactions, traces, logs, and outputs                                                             |
| -i  | --cache_txs                | write transactions to the cache (see notes)                                                                    |
| -R  | --cache_traces             | write traces to the cache (see notes)                                                                          |
| -y  | --factory                  | scan for contract creations from the given address(es)<br/>and report address of those contracts               |
|     | --emitter                  | for log export only, export only if one of the given<br/>export addresses emitted the evt                      |
|     | --source &lt;addr&gt;      | for log export only, export only one of these addresses<br/>emitted the event                                  |
|     | --relevant                 | for log and accounting export only, if true export<br/>only logs relevant to one of the given export addresses |
| -U  | --count                    | only available for --appearances mode, if present return<br/>only the number of records                        |
| -c  | --first_record &lt;num&gt; | the first record to process                                                                                    |
| -e  | --max_records &lt;num&gt;  | the maximum number of records to process before reporting                                                      |
|     | --clean                    | clean (i.e. remove duplicate appearances) from all<br/>existing monitors                                       |
| -x  | --fmt &lt;val&gt;          | export format, one of [none, json, txt, csv, api]                                                              |
| -v  | --verbose                  | set verbose level (optional level defaults to 1)                                                               |
| -h  | --help                     | display this help screen                                                                                       |
{{</td>}}

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

`Configurable Items:`

`cache_txs`: write transactions to the cache (see notes).
`cache_traces`: write traces to the cache (see notes).
`skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).
`max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction
  is (default = 250).

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

