## chifra receipts

`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using `chifra transactions` and more focused data using `chifra logs`. It is included here for completeness, as the `receipt` is a fundamental data structure in Ethereum.

### Usage

`Usage:`    chifra receipts [-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve a transaction's receipt from the cache or the node.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | transactions | a space-separated list of one or more transaction identifiers (required) |
| -a | --articulate | articulate the retrieved data if ABIs can be found |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash, 
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.

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

**Source code**: [`tools/getReceipts`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getReceipts)

