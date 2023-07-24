## chifra blocks

<!-- markdownlint-disable MD041 -->
The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the
TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts).
You may optionally retrieve only the transaction hashes in the block (which is significantly faster).
Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block.
TrueBlocks uses a similar feature internally to build its index of appearances. This type of data
is very insightful when studying end user behavior and chain-wide adoption analysis.

```[plaintext]
Purpose:
  Retrieve one or more blocks from the chain or local cache.

Usage:
  chifra blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)

Flags:
  -e, --hashes            display only transaction hashes, default is to display full transaction detail
  -c, --uncles            display uncle blocks (if any) instead of the requested block
  -t, --traces            export the traces from the block as opposed to the block data
  -u, --uniq              display a list of uniq address appearances per transaction
  -f, --flow string       for the uniq option only, export only from or to (including trace from or to)
                          One of [ from | to | reward ]
  -l, --logs              display only the logs found in the block(s)
  -m, --emitter strings   for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings     for the --logs option only, filter logs to show only those with this topic(s)
  -a, --articulate        for the --logs option only, articulate the retrieved data if ABIs can be found
  -r, --big_range uint    for the --logs option only, allow for block ranges larger than 500 (default 500)
  -U, --count             display the number of the lists of appearances for --addrs or --uniq
  -D, --decache           removes a block and any transactions or traces in the block from the cache
  -w, --raw               report JSON data from the source with minimal processing
  -o, --cache             force the results of the query into the cache
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose (increase detail with --log_level)
  -h, --help              display this help screen

Notes:
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - Blocks may be specified as either numbers or hashes.
  - Special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - For the --logs option, large block ranges may crash the node, use --big_range to specify a larger range.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [block](/data-model/chaindata/#block)
- [log](/data-model/chaindata/#log)
- [logfilter](/data-model/chaindata/#logfilter)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [blockcount](/data-model/chaindata/#blockcount)

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
  ```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

