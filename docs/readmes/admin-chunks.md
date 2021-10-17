## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks [flags] <block> [block...]

Arguments:
  blocks - an optional list of blocks to process

Flags:
  -l, --list             list the bloom and index hashes from local cache or IPFS
  -c, --check            check the validity of the chunk or bloom
  -e, --extract string   show some or all of the contents of the chunk or bloom filters
                         One of [ header | addr_table | app_table | chunks | blooms ]
  -s, --stats            for the --list option only, display statistics about each chunk or bloom
  -a, --save             for the --extract option only, save the entire chunk to a similarly named file as well as display

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - Only a single block in a given chunk needs to be supplied.
```

**Source code**: [`apps/chunkMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chunkMan)

