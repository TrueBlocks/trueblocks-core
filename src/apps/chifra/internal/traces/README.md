## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated string of the same values used by `trace_fitler`.

```[plaintext]
Purpose:
  Retrieve traces for the given transaction(s).

Usage:
  chifra traces [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate      articulate the retrieved data if ABIs can be found
  -f, --filter string   call the node's trace_filter routine with bang-separated filter
  -d, --statediff       export state diff traces (not implemented)
  -c, --count           show the number of traces for the transaction only (fast)
  -x, --fmt string      export format, one of [none|json*|txt|csv|api]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
  - A state diff trace describes, for each modified address, what changed during that trace.
```

Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --dollars         export values in US dollars
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
