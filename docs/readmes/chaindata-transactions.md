## chifra transactions

The `chifra transactions` tool retrieves transactions directly from the Ethereum node (using the `--raw` option) or from the TrueBlocks cache (if present). You may specify multiple transaction identifiers per invocation. Unlike the Ethereum RPC, the reported transactions include the transaction's receipt and generated logs.

The `--articulate` option fetches the ABI from each encountered smart contract (including those encountered in a trace--if the `--trace` option is enabled) to better describe the reported data.

The `--trace` option attaches an array transaction traces to the output (if the node you're querying has --tracing enabled), while the `--uniq` option displays a list of uniq address appearances instead of the underlying data (including uniq addresses in traces if enabled).

```[plaintext]

```

**Source code**: [`internal/transactions`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/transactions)

