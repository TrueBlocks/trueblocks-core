## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with `chifra scrape` and the list of transaction identifiers created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's `source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`, `--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you.

```[plaintext]
Purpose:
  Export full detail of transactions for one or more addresses.

Usage:
  chifra export [flags] <address> [address...] [topics...] [fourbytes...]

Arguments:
  addrs - one or more addresses (0x...) to export (required)
  topics - filter by one or more log topics (only for --logs option)
  fourbytes - filter by one or more fourbytes (only for transactions and trace options)

Flags:
  -p, --appearances         export a list of appearances
  -r, --receipts            export receipts instead of transaction list
  -A, --statements          for use with --accounting option only, export only reconciliation statements
  -l, --logs                export logs instead of transaction list
  -t, --traces              export traces instead of transaction list
  -C, --accounting          export accounting records instead of transaction list
  -a, --articulate          articulate transactions, traces, logs, and outputs
  -i, --cache_txs           write transactions to the cache (see notes)
  -R, --cache_traces        write traces to the cache (see notes)
  -y, --factory             scan for contract creations from the given address(es) and report address of those contracts
  -U, --count               only available for --appearances mode, if present, return only the number of records
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process before reporting (default 250)
      --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
      --emitter strings     for log export only, export only logs if emitted by one of these address(es)
      --topic strings       for log export only, export only logs with this topic(s)
      --clean               clean (i.e. remove duplicate appearances) from all existing monitors

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
```

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)

