## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks [flags] <block> [block...]

Arguments:
  blocks - optional list of blocks to intersect with chunk ranges

Flags:
  -e, --extract string   show some or all of the contents of the chunk or bloom filters
                         One of [ stats | pins | blooms | index | header | addresses | appearances ]
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen

Notes:
  - Only a single block in a given chunk needs to be supplied.
```

**Source code**: [`internal/chunks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

