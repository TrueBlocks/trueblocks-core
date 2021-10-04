## chifra pins

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
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
