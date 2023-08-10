## chifra transactions

<!-- markdownlint-disable MD041 -->
The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw`
option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers
per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt
and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those
encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attaches an array transaction traces to the output (if the node you're querying
has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances
instead of the underlying data (including uniq addresses in traces if enabled).

```[plaintext]

```

Data models produced by this tool:

- [statement](/data-model/accounts/#statement)
- [transaction](/data-model/chaindata/#transaction)
- [transfer](/data-model/chaindata/#transfer)

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
  ```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

