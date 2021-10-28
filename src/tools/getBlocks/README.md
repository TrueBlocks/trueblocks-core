## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts). You may optionally retrieve only the transaction hashes in the block (which is significantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behavior and chain-wide adoption analysis.

```[plaintext]
Purpose:
  Retrieve one or more blocks from the chain or local cache.

Usage:
  chifra blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)

Flags:
  -e, --hashes    display only transaction hashes, default is to display full transaction detail
  -U, --uncles    display uncle blocks (if any) instead of the requested block
  -t, --trace     export the traces from the block as opposed to the block data
  -s, --apps      display only the list of address appearances in the block
  -u, --uniq      display only the list of uniq address appearances in the block
  -n, --uniq_tx   display only the list of uniq address appearances in each transaction
  -c, --count     display the number of the lists of appearances for --apps, --uniq, or --uniq_tx
  -o, --cache     force a write of the block to the cache

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - blocks may be specified as either numbers or hashes.
  - special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emmitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - Large block ranges may crash the node, use --big_range to specify a larger range.
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
