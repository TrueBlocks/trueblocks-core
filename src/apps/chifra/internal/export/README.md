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
  -r, --receipts            export receipts instead of transactional data
  -l, --logs                export logs instead of transactional data
  -t, --traces              export traces instead of transactional data
  -A, --statements          export reconciliations instead of transactional data (requires --accounting option)
  -n, --neighbors           export the neighbors of the given address
  -C, --accounting          attach accounting records to the exported data (applies to transactions export only)
  -a, --articulate          articulate transactions, traces, logs, and outputs
  -i, --cache               write transactions to the cache (see notes)
  -R, --cache_traces        write traces to the cache (see notes)
  -y, --factory             scan for contract creations from the given address(es) and report address of those contracts
  -U, --count               only available for --appearances mode, if present, return only the number of records
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process before reporting (default 250)
      --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
      --emitter strings     for log export only, export only logs if emitted by one of these address(es)
      --topic strings       for log export only, export only logs with this topic(s)
      --asset strings       for the statements option only, export only reconciliations for this asset
      --clean               clean (i.e. remove duplicate appearances) from all existing monitors
  -s, --staging             export transactions labeled staging (i.e. older than 28 blocks but not yet consolidated)
  -u, --unripe              export transactions labeled upripe (i.e. less than 28 blocks old)
  -F, --first_block uint    first block to process (inclusive)
  -L, --last_block uint     last block to process (inclusive)
  -x, --fmt string          export format, one of [none|json*|txt|csv|api]
  -v, --verbose             enable verbose (increase detail with --log_level)
  -h, --help                display this help screen

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.
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
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
