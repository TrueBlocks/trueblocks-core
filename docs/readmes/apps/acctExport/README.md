## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

### usage

`Usage:`    chifra export [-p|-r|-l|-t|-C|-A|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]
`Purpose:`  Export full detail of transactions for one or more addresses.

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
| -A | --statements | for use with --accounting option only, export only reconciliation statements |
| -a | --articulate | articulate transactions, traces, logs, and outputs |
| -i | --cache_txs | write transactions to the cache (see notes) |
| -R | --cache_traces | write traces to the cache (see notes) |
| -y | --factory | scan for contract creations from the given address(es) and report address of those contracts |
|  | --emitter | for log export only, export only if one of the given export addresses emitted the event |
|  | --source &lt;addr&gt; | for log export only, export only one of these addresses emitted the event |
|  | --relevant | for log and accounting export only, if true export only logs relevant to one of the given export addresses |
| -U | --count | only available for --appearances mode, if present return only the number of records |
| -c | --first_record &lt;num&gt; | the first record to process |
| -e | --max_records &lt;num&gt; | the maximum number of records to process before reporting |
|  | --clean | clean (i.e. remove duplicate appearances) from all existing monitors |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

