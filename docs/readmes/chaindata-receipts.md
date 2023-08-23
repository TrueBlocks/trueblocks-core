## chifra receipts

<!-- markdownlint-disable MD041 -->
`chifra receipts` returns the given transaction's receipt. You may specify multiple transaction identifiers
per invocation.

The `--articulate` option fetches the ABI from each encountered smart contract (including those
encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

Generally speaking, this tool is less useful than others as you may report the same data using
`chifra transactions` and more focused data using `chifra logs`. It is included here for
completeness, as the `receipt` is a fundamental data structure in Ethereum.

```[plaintext]
Purpose:
  Retrieve receipts for the given transaction(s).

Usage:
  chifra receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate   articulate the retrieved data if ABIs can be found
  -w, --raw          report JSON data from the source with minimal processing
  -o, --cache        force the results of the query into the cache
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
```

Data models produced by this tool:

- [receipt](/data-model/chaindata/#receipt)

Links:

- [api docs](/api/#operation/chaindata-receipts)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/receipts)

