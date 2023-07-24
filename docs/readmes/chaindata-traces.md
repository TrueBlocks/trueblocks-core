## chifra traces

<!-- markdownlint-disable MD041 -->
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
  -U, --count           show the number of traces for the transaction only (fast)
  -w, --raw             report JSON data from the source with minimal processing
  -o, --cache           force the results of the query into the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - A bang separated filter has the following fields (at least one of which is required) and is separated with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
```

Data models produced by this tool:

- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [tracecount](/data-model/chaindata/#tracecount)
- [tracefilter](/data-model/chaindata/#tracefilter)

Links:

- [api docs](/api/#operation/chaindata-traces)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/traces)

