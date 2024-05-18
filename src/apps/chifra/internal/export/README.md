## chifra export

The `chifra export` tools provides a major part of the functionality of the TrueBlocks system. Using
the index of appearances created with `chifra scrape` and the list of transaction identifiers
created with `chifra list`, `chifra export` completes the actual extraction of an address's transactional
history from the node.

You may use `topics`, `fourbyte` values at the start of a transaction's input data, and/or a log's
`source address` or `emitter` to filter the results.

You may also choose which portions of the Ethereum data structures (`--transactions`, `--logs`,
`--traces`, etc.) as you wish.

By default, the results of the extraction are delivered to your console, however, you may export
the results to any database (with a little bit of work). The format of the data, its content and
its destination are up to you.

```[plaintext]
Purpose:
  Export full details of transactions for one or more addresses.

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
  -n, --neighbors           export the neighbors of the given address
  -C, --accounting          attach accounting records to the exported data (applies to transactions export only)
  -A, --statements          for the accounting options only, export only statements
  -b, --balances            traverse the transaction history and show each change in ETH balances
  -i, --withdrawals         export withdrawals for the given address
  -a, --articulate          articulate transactions, traces, logs, and outputs
  -R, --cache_traces        force the transaction's traces into the cache
  -U, --count               for --appearances mode only, display only the count of records
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 250)
  -N, --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
  -m, --emitter strings     for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings       for the --logs option only, filter logs to show only those with this topic(s)
  -V, --reverted            export only transactions that were reverted
  -P, --asset strings       for the accounting options only, export statements only for this asset
  -f, --flow string         for the accounting options only, export statements with incoming, outgoing, or zero value
                            One of [ in | out | zero ]
  -y, --factory             for --traces only, report addresses created by (or self-destructed by) the given address(es)
  -u, --unripe              export transactions labeled unripe (i.e. less than 28 blocks old)
  -E, --reversed            produce results in reverse chronological order
  -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
  -F, --first_block uint    first block to process (inclusive)
  -L, --last_block uint     last block to process (inclusive)
  -H, --ether               specify value in ether
  -o, --cache               force the results of the query into the cache
  -D, --decache             removes related items from the cache
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.
  - If present, --first_/--last_block are applied, followed by user-supplied filters such as asset or topic, followed by --first_/--max_record if present.
  - The --first_record and --max_record options are zero-based (as are the block options).
  - The _block and _record filters are ignored when used with the --count option.
  - If the --reversed option is present, the appearance list is reversed prior to all processing (including filtering).
  - The --decache option will remove all cache items (blocks, transactions, traces, etc.) for the given address(es).
  - The --withdrawals option is only available on certain chains. It is ignored otherwise.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [appearancecount](/data-model/accounts/#appearancecount)
- [function](/data-model/other/#function)
- [log](/data-model/chaindata/#log)
- [message](/data-model/other/#message)
- [monitor](/data-model/accounts/#monitor)
- [parameter](/data-model/other/#parameter)
- [receipt](/data-model/chaindata/#receipt)
- [statement](/data-model/accounts/#statement)
- [token](/data-model/chainstate/#token)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [transaction](/data-model/chaindata/#transaction)
- [withdrawal](/data-model/chaindata/#withdrawal)

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
