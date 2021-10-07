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
  -f, --freshen       check for new bloom or index chunks and download if available
  -a, --all           in addition to Bloom filters, download full index chunks
  -S, --share         share downloaded data by pinning it to IPFS (the IPFS daemon must be running)
  -r, --remote        for --list mode only, recover the manifest from IPFS via UnchainedIndex smart contract
  -s, --sleep float   throttle requests by this many seconds (.25 seconds delay between requests by default) (default 0.25)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The --freshen option is similar to --init, but checks UnchainedIndex first.
  - One of --list, --init, or --freshen is required.
  - The --share option only works if the IPFS daemon is running.
  - Re-run chifra init as you wish. It will repair or freshen the index.
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
