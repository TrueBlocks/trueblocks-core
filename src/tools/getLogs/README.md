## chifra logs

`chifra logs` returns the given transaction's logs. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data. The `--topic` and `--source` options allow you to filter your results.

### Usage

`Usage:`    chifra logs [-t|-s|-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve logs for the given transaction(s).

`Where:`

|          | Option                            | Description                                                                  |
| -------- | --------------------------------- | ---------------------------------------------------------------------------- |
|          | transactions                      | a space-separated list of one or more transaction identifiers<br/>(required) |
| &#8208;t | &#8208;&#8208;topic &lt;hash&gt;  | filter by one or more log topics (not implemented)                           |
| &#8208;s | &#8208;&#8208;source &lt;addr&gt; | export only if the given address emitted the event<br/>(not implemented)     |
| &#8208;a | &#8208;&#8208;articulate          | articulate the retrieved data if ABIs can be found                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;     | export format, one of [none, json, txt, csv, api]                            |
| &#8208;v | &#8208;&#8208;verbose             | set verbose level (optional level defaults to 1)                             |
| &#8208;h | &#8208;&#8208;help                | display this help screen                                                     |

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.
- If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is
  not found, it will be used as a topic.

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

**Source code**: [`tools/getLogs`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getLogs)

