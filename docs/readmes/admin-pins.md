## chifra pins

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage pinned index of appearances and associated blooms.

Usage:
  chifra pins [flags]

Flags:
  -l, --list          list the bloom and index hashes from local cache or IPFS
  -i, --init          download the blooms or index chunks from IPFS
  -a, --all           in addition to Bloom filters, download full index chunks
  -S, --share         share downloaded data by pinning it to IPFS (the IPFS daemon must be running)
  -s, --sleep float   throttle requests by this many seconds (default 0.25)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.
```

**Source code**: [`pins.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/internal/pins.go)

