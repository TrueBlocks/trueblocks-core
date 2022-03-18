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
  -x, --fmt string    export format, one of [none|json*|txt|csv|api]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.
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
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
