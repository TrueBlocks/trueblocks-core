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
  -e, --hashes        display only transaction hashes, default is to display full transaction detail
  -c, --uncles        display uncle blocks (if any) instead of the requested block
  -t, --traces        export the traces from the block as opposed to the block data
  -s, --apps          display a list of uniq address appearances in the block
  -u, --uniq          display a list of uniq address appearances per transaction
  -f, --flow string   for the uniq and apps options only, export only from or to (including trace from or to)
                      One of [ from | to | reward ]
  -U, --count         display the number of the lists of appearances for --addrs or --uniq
  -o, --cache         force a write of the block to the cache
  -D, --decache       removes a block and any transactions or traces in the block from the cache
  -x, --fmt string    export format, one of [none|json*|txt|csv]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - Blocks may be specified as either numbers or hashes.
  - Special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - Large block ranges may crash the node, use --big_range to specify a larger range.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [block](/data-model/chaindata/#block)
- [log](/data-model/chaindata/#log)
- [trace](/data-model/chaindata/#trace)
- [traceaction](/data-model/chaindata/#traceaction)
- [traceresult](/data-model/chaindata/#traceresult)
- [blockcount](/data-model/chaindata/#blockcount)

Links:

- [api docs](/api/#operation/chaindata-blocks)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/blocks)

