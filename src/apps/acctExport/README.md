## acctExport

This folder contains a QBlocks monitor. QBlocks monitors pull transactions from the Ethereum blockchain for a given (or a series of) Ethereum addresses.

Below we present the command line interface to this tool, although the tool itself is not available under open source. While the tool is in active development, QBlocks monitors already produce very useful results. For example, we use QBlocks monitors to account for and analyze all transactions on a given smart contract. We present [this example](http://dao.quickblocks.io).

Please contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.

#### Usage

`Usage:` acctExport [-p|-r|-l|-t|-b|-e|-c|-a|-w|-i|-R|-v|-h] &lt;address&gt; [address...]  
`Purpose:` Export full detail of transactions for one or more Ethereum addresses.

`Where:`

| Short Cut | Option         | Description                                                        |
| --------: | :------------- | :----------------------------------------------------------------- |
|           | addrs          | one or more addresses (0x...) to export (required)                 |
|        -p | --appearances  | export a list of appearances                                       |
|        -r | --receipts     | export receipts instead of transaction list                        |
|        -l | --logs         | export logs instead of transaction list                            |
|        -t | --traces       | export traces instead of transaction list                          |
|        -b | --balances     | export balance history instead of transaction list                 |
|        -a | --articulate   | articulate transactions, traces, logs, and outputs                 |
|        -i | --write_txs    | write transactions to the cache (see notes)                        |
|        -R | --write_traces | write traces to the cache (see notes)                              |
|        -v | --verbose      | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
|        -h | --help         | display this help screen                                           |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- By default, transactions and traces are cached if the number of exported
  appearances is <= to 1,000 appearances. Otherwise, if you specify any `write_*` options,
  your preference predominates.

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**
