## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

### Usage

`Usage:`    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more addresses.

`Where:`

|          | Option                                 | Description                                                                                                    |
| -------- | -------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
|          | addrs                                  | one or more addresses (0x...) to export (required)                                                             |
|          | topics                                 | filter by one or more log topics (only for --logs option)                                                      |
|          | fourbytes                              | filter by one or more fourbytes (only for transactions<br/>and trace options)                                  |
| &#8208;p | &#8208;&#8208;appearances              | export a list of appearances                                                                                   |
| &#8208;r | &#8208;&#8208;receipts                 | export receipts instead of transaction list                                                                    |
| &#8208;A | &#8208;&#8208;statements               | for use with --accounting option only, export only<br/>reconciliation statements                               |
| &#8208;l | &#8208;&#8208;logs                     | export logs instead of transaction list                                                                        |
| &#8208;t | &#8208;&#8208;traces                   | export traces instead of transaction list                                                                      |
| &#8208;C | &#8208;&#8208;accounting               | export accounting records instead of transaction list                                                          |
| &#8208;a | &#8208;&#8208;articulate               | articulate transactions, traces, logs, and outputs                                                             |
| &#8208;i | &#8208;&#8208;cache_txs                | write transactions to the cache (see notes)                                                                    |
| &#8208;R | &#8208;&#8208;cache_traces             | write traces to the cache (see notes)                                                                          |
| &#8208;y | &#8208;&#8208;factory                  | scan for contract creations from the given address(es)<br/>and report address of those contracts               |
|          | &#8208;&#8208;emitter                  | for log export only, export only if one of the given<br/>export addresses emitted the event                    |
|          | &#8208;&#8208;source &lt;addr&gt;      | for log export only, export only one of these addresses<br/>emitted the event                                  |
|          | &#8208;&#8208;relevant                 | for log and accounting export only, if true export<br/>only logs relevant to one of the given export addresses |
| &#8208;U | &#8208;&#8208;count                    | only available for --appearances mode, if present return<br/>only the number of records                        |
| &#8208;c | &#8208;&#8208;first_record &lt;num&gt; | the first record to process                                                                                    |
| &#8208;e | &#8208;&#8208;max_records &lt;num&gt;  | the maximum number of records to process before reporting                                                      |
|          | &#8208;&#8208;clean                    | clean (i.e. remove duplicate appearances) from all<br/>existing monitors                                       |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;          | export format, one of [none, json, txt, csv, api]                                                              |
| &#8208;v | &#8208;&#8208;verbose                  | set verbose level (optional level defaults to 1)                                                               |
| &#8208;h | &#8208;&#8208;help                     | display this help screen                                                                                       |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

`Configurable Items:`

- `cache_txs`: write transactions to the cache (see notes).
- `cache_traces`: write traces to the cache (see notes).
- `skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).
- `max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction
  is (default = 250).

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --nocolor   | turn off colored display                                      |
| --wei       | specify value in wei (the default)                            |
| --ether     | specify value in ether                                        |
| --dollars   | specify value in US dollars                                   |
| --raw       | report JSON data from the node with minimal processing        |
| --very_raw  | report JSON data from node with zero processing               |
| --to_file   | write the results to a temporary file and return the filename |
| --output:fn | write the results to file 'fn' and return the filename        |
| --file:fn   | specify multiple sets of command line options in a file.      |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

