## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use a log's `topics`, the `fourbyte` values at the head of a transaction's input data, and/or a log's `source address` in order to filter your results.

You may also choose which portions of the Ethereum data structures (transactions, logs, traces, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

### Usage

`Usage:`    chifra export [-p|-r|-l|-t|-C|-O|-a|-i|-R|-d|-m|-f|-y|-U|-c|-e|-s|-u|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more Ethereum addresses.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | addrs | one or more addresses (0x...) to export (required) |
|  | topics | filter by one or more log topics (only for --logs option) |
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
| -d | --skip_ddos | toggle skipping over 2016 dDos transactions ('on' by default) |
| -m | --max_traces &lt;num&gt; | if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250) |
| -f | --freshen | freshen but do not print the exported data |
| -y | --factory | scan for contract creations from the given address(es) and report address of those contracts |
|  | --emitter | for log export only, export only if one of the given export addresses emitted the event |
|  | --source &lt;addr&gt; | for log export only, export only one of these addresses emitted the event |
|  | --relevant | for log export only, if true export only logs relevant to one of the given export addresses |
| -U | --count | only available for --appearances mode, if present return only the number of records |
| -c | --first_record &lt;num&gt; | the first record to process |
| -e | --max_records &lt;num&gt; | the maximum number of records to process before reporting |
|  | --clean | clean (i.e. remove duplicate appearances) from all existing monitors |
| -s | --staging | enable search of staging (not yet finalized) folder |
| -u | --unripe | enable search of unripe (neither staged nor finalized) folder (assumes --staging) |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

