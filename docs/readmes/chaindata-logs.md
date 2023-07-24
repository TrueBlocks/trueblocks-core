## chifra logs

<!-- markdownlint-disable MD041 -->
`chifra logs` returns the given transaction's logs. You may specify multiple transaction identifiers
per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract to better describe
the reported data. The `--topic` and `--source` options allow you to filter your results.

```[plaintext]
Purpose:
  Retrieve logs for the given transaction(s).

Usage:
  chifra logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate   articulate the retrieved data if ABIs can be found
  -w, --raw          report JSON data from the source with minimal processing
  -o, --cache        force the results of the query into the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if it is not, the hash will be used as a topic.
```

Data models produced by this tool:

- [log](/data-model/chaindata/#log)
- [logfilter](/data-model/chaindata/#logfilter)

Links:

- [api docs](/api/#operation/chaindata-logs)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/logs)

