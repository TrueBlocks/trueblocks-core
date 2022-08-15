## chifra chunks

The chifra chunks routine provides tools for interacting with, checking the validity of,
cleaning up, and analyizing the Unchained Index. It provides options to list pins,
the Manifest, summary data on the index, Bloom filters, addresses, and appearances.
While still in its early stages, this tool will eventually allow users to clean
their local index, clean their remote index, study the indexes, etc. Stay tuned.

```[plaintext]
Purpose:
  Manage, investigate, and display the Unchained Index.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of information to retrieve (required)
	One of [ status | index | blooms | manifest | stats | addresses | appearances ]
  blocks - optional list of blocks used to intersect with chunk ranges
  addrs - optional list of addresses for use with --belongs option (see notes)

Flags:
  -i, --pin          pin all chunks (locally if IPFS daemon is running, and/or remotely with --remote flag)
  -m, --remote       for some options, forces processing to use remote data
  -p, --publish      repin chunks, pin the manifest, and publish to the Unchained Index smart contract
  -b, --belongs      in index mode only, checks if the given address appears in the given chunk
  -c, --check        in index mode only, checks for internal consistency of the index, blooms, and manifest
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - Only a single block in a given chunk needs to be supplied for a match.
  - The --belongs option is only available with the addresses or blooms mode.
  - The --belongs option requires both an address and a block identifier.
  - You may only specifiy an address when using the --belongs option.
  - The --pin, --publish, --truncate, --repair, and --check option are available only in index mode.
  - The --repair and --truncate options update the manifest, but do not publish it.
```

#### Other Options

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
