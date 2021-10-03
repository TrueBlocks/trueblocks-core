## chifra pins

This tool is not yet ready for production use. Please return to this page later.

```
Purpose:
  Manage pinned index of appearances and associated Bloom filters.

Usage:
  chifra pins [flags]

Flags:
  -l, --list      list the index and Bloom filter hashes from local manifest or pinning service
  -i, --init      initialize index of appearances by downloading Bloom filters from pinning service
  -f, --freshen   freshen index of appearances using the same mode from most recent --init
  -a, --all       for --init and --freshen modes only, download full index chunks as well as Bloom filter
  -S, --share     pin downloaded files to your local IPFS store, that is, share them (requires IPFS)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - One of --list, --init, or --freshen is required.
  - the --share option only works if the IPFS executable is in your path.
```
[{FOOTER}]
