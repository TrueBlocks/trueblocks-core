## chifra pins

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage pinned index of appearances and associated blooms.

Usage:
  chifra pins [flags]

Flags:
  -i, --init         download the blooms or index chunks from IPFS
  -a, --all          in addition to Bloom filters, download full index chunks
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.
```

**Source code**: [`internal/pins`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/pins)

