acctExport argc: 2 [1:-th] 
acctExport -th 
### Usage

`Usage:`    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] &lt;address&gt; [address...] [topics] [fourbytes]  
`Purpose:`  Export full detail of transactions for one or more addresses.

`Where:`  

|     | Option                     | Description                                                                                                                                                     |
| --- | -------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|     | addrs                      | one or more addresses (0x...) to export (required)                                                                                                              |
|     | topics                     | filter by one or more log topics (only for --logs option)                                                                                                       |
|     | fourbytes                  | filter by one or more fourbytes (only for transactions and trace options)                                                                                       |
| -p  | --appearances              | export a list of appearances                                                                                                                                    |
| -r  | --receipts                 | export receipts instead of transaction list                                                                                                                     |
| -A  | --statements               | for use with --accounting option only, export only reconciliation statements                                                                                    |
| -l  | --logs                     | export logs instead of transaction list                                                                                                                         |
| -t  | --traces                   | export traces instead of transaction list                                                                                                                       |
| -C  | --accounting               | export accounting records instead of transaction list                                                                                                           |
| -a  | --articulate               | articulate transactions, traces, logs, and outputs                                                                                                              |
| -i  | --cache_txs                | write transactions to the cache (see notes)                                                                                                                     |
| -R  | --cache_traces             | write traces to the cache (see notes)                                                                                                                           |
| -y  | --factory                  | scan for contract creations from the given address(es) and report address of those contracts                                                                    |
|     | --emitter                  | for log export only, export only if one of the given export addresses emitted the event                                                                         |
|     | --source &lt;addr&gt;      | for log export only, export only one of these addresses emitted the event                                                                                       |
|     | --relevant                 | for log and accounting export only, if true export only logs relevant to one of the given export addresses                                                      |
| -U  | --count                    | only available for --appearances mode, if present return only the number of records                                                                             |
| -c  | --first_record &lt;num&gt; | the first record to process                                                                                                                                     |
| -e  | --max_records &lt;num&gt;  | the maximum number of records to process before reporting                                                                                                       |
|     | --clean                    | clean (i.e. remove duplicate appearances) from all existing monitors                                                                                            |
| ### | Hidden options             |                                                                                                                                                                 |
| -f  | --freshen                  | freshen but do not print the exported data                                                                                                                      |
| -F  | --first_block &lt;num&gt;  | first block to process (inclusive)                                                                                                                              |
| -L  | --last_block &lt;num&gt;   | last block to process (inclusive)                                                                                                                               |
| -s  | --staging                  | enable search of staging (not yet finalized) folder                                                                                                             |
| -u  | --unripe                   | enable search of unripe (neither staged nor finalized) folder (assumes --staging)                                                                               |
|     | --load &lt;str&gt;         | a comma separated list of dynamic traversers to load                                                                                                            |
|     | --reversed                 | produce results in reverse chronological order                                                                                                                  |
| -b  | --by_date                  | produce results sorted by date (default is to report by address)                                                                                                |
| -z  | --summarize_by &lt;val&gt; | for --accounting only, summarize reconciliations by this time period, one of *[ yearly \| quarterly \| monthly \| weekly \| daily \| hourly \| blockly \| tx ]* |
| ### | Hidden options             |                                                                                                                                                                 |
| -x  | --fmt &lt;val&gt;          | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                                                                                                 |
| -v  | --verbose                  | set verbose level (optional level defaults to 1)                                                                                                                |
| -h  | --help                     | display this help screen                                                                                                                                        |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.

`Configurable Items:`

`cache_txs`: write transactions to the cache (see notes).
`cache_traces`: write traces to the cache (see notes).
`skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).
`max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction
  is (default = 250).

