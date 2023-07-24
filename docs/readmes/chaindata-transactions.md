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
Purpose:
  Retrieve one or more transactions from the chain or local cache.

Usage:
  chifra transactions [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)

Flags:
  -a, --articulate           articulate the retrieved data if ABIs can be found
  -t, --traces               include the transaction's traces in the results
  -u, --uniq                 display a list of uniq addresses found in the transaction
  -f, --flow string          for the uniq option only, export only from or to (including trace from or to)
                             One of [ from | to ]
  -l, --logs                 display only the logs found in the transaction(s)
  -m, --emitter strings      for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings        for the --logs option only, filter logs to show only those with this topic(s)
  -A, --account_for string   reconcile the transaction as per the provided address
  -D, --decache              removes a transactions and any traces in the transaction from the cache
  -H, --ether                specify value in ether
  -w, --raw                  report JSON data from the source with minimal processing
  -o, --cache                force the results of the query into the cache
  -x, --fmt string           export format, one of [none|json*|txt|csv]
  -v, --verbose              enable verbose (increase detail with --log_level)
  -h, --help                 display this help screen

Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - The --traces option, when used with --account_for, will descend into traces to complete reconciliations.
```

Data models produced by this tool:

- [statement](/data-model/accounts/#statement)
- [transaction](/data-model/chaindata/#transaction)
- [transfer](/data-model/chaindata/#transfer)

Links:

- [api docs](/api/#operation/chaindata-transactions)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/transactions)

