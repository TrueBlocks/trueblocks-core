## chifra traces

The `chifra traces` tool retrieves a transaction's traces. You may specify multiple transaction
identifiers per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe
the reported data.

The `--filter` option calls your node's `trace_filter` routine (if available) using a bang-separated
string of the same values used by `trace_fitler`.

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
  -U, --count           display only the number of traces for the transaction (fast)
  -H, --ether           specify value in ether
  -o, --cache           force the results of the query into the cache
  -D, --decache         removes related items from the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose output
  -h, --help            display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
```

Data models produced by this tool:

- [function](/data-model/other/#function)
- [message](/data-model/other/#message)
- [parameter](/data-model/other/#parameter)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [tracecount](/data-model/chaindata/#tracecount)
- [tracefilter](/data-model/chaindata/#tracefilter)
- [traceresult](/data-model/chaindata/#traceresult)

### further information

The `--traces` option requires your node to enable the `trace_block` (and related) RPC endpoints. Many remote RPC providers do not enable these endpoints due to the additional load they can place on the node. If you are running your own node, you can enable these endpoints by adding `trace` to your node's startup.

The test for tracing assumes your node provides tracing starting at block 1. If your is partially synced, you may export the following enviroment variable before running the command to instruct `chifra` where to test.

```[bash]
export TB_<chain>_FIRSTTRACE=<bn>
```

where `<chain>` is the chain you are running and `<bn>` is the block number at which tracing starts. For example, to start tracing at block 1000 on the mainnet, you would export `TB_MAINNET_FIRSTTRACE=1000`.

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

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
