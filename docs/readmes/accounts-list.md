## chifra list

<!-- markdownlint-disable MD041 -->
`chifra list` takes one or more addresses, queries the index of appearances, and builds TrueBlocks
monitors. A TrueBlocks monitor is a file that contains blockNumber.transactionId pairs (transaction
identifiers) representing the history of the address.

Because TrueBlocks only extracts data from the Ethereum node when it's requested, the first time
you list an address it takes about a minute. Subsequent queries are much faster because TrueBlocks
caches the results.

Note that `chifra list` only queries the index, it does not extract the full transactional details.
You may use `chifra export` for that.

```[plaintext]
Purpose:
  List every appearance of an address anywhere on the chain.

Usage:
  chifra list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)

Flags:
  -U, --count               display only the count of records for each monitor
  -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
  -b, --bounds              report first and last block this address appears
  -u, --unripe              list transactions labeled upripe (i.e. less than 28 blocks old)
  -s, --silent              freshen the monitor only (no reporting)
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 250)
  -E, --reversed            produce results in reverse chronological order
  -F, --first_block uint    first block to export (inclusive, ignored when freshening)
  -L, --last_block uint     last block to export (inclusive, ignored when freshening)
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - No other options are permitted when --silent is selected.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [monitor](/data-model/accounts/#monitor)
- [appearancecount](/data-model/accounts/#appearancecount)
- [bounds](/data-model/accounts/#bounds)

Links:

- [api docs](/api/#operation/accounts-list)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/list)

