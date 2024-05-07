## chifra blocks

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
  -f, --flow string       for the --uniq option only, export only from or to (including trace from or to)
                          One of [ from | to | reward ]
  -l, --logs              display only the logs found in the block(s)
  -m, --emitter strings   for the --logs option only, filter logs to show only those logs emitted by the given address(es)
  -B, --topic strings     for the --logs option only, filter logs to show only those with this topic(s)
  -i, --withdrawals       export the withdrawals from the block as opposed to the block data
  -a, --articulate        for the --logs option only, articulate the retrieved data if ABIs can be found
  -U, --count             display only the count of appearances for --addrs or --uniq
  -H, --ether             specify value in ether
  -w, --raw               report JSON data from the source with minimal processing
  -o, --cache             force the results of the query into the cache
  -D, --decache           removes related items from the cache
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - Blocks may be specified as either numbers or hashes.
  - Special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - The --decache option removes the block(s), all transactions in those block(s), and all traces in those transactions from the cache.
  - The --withdrawals option is only available on certain chains. It is ignored otherwise.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [block](/data-model/chaindata/#block)
- [blockcount](/data-model/chaindata/#blockcount)
- [log](/data-model/chaindata/#log)
- [logfilter](/data-model/chaindata/#logfilter)
- [message](/data-model/other/#message)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
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
