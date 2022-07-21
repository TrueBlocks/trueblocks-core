## chifra chunks

The chifra chunks routine provides tools for interacting with, checking the validity of,
cleaning up, and analyizing the Unchained Index. It provides options to list pins,
the Manifest, summary data on the index, Bloom filters, addresses, and appearances.
While still in its early stages, this tool will eventually allow users to clean
their local index, clean their remote index, study the indexes, etc. Stay tuned.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of chunk info to retrieve (required)
	One of [ stats | manifest | pins | blooms | index | addresses | appearances ]
  blocks - optional list of blocks to intersect with chunk ranges
  addrs - one or more addresses to use with --belongs option (see note)

Flags:
  -d, --details      for manifest and addresses options only, display full details of the report
  -c, --check        depends on mode, checks for internal consistency of the data type
  -b, --belongs      checks if the given address appears in the given chunk
  -n, --clean        retrieve all pins on Pinata, compare to manifest, remove any extraneous remote pins
  -m, --remote       for some options, force processing from remote data
  -i, --pin_remote   pin any previously unpinned chunks and blooms to a remote pinning service
  -p, --publish      update the manifest and publish it to the Unchained Index smart contract
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - Only a single block in a given chunk needs to be supplied for a match.
  - The --belongs option is only available with the addresses or blooms mode.
  - The --belongs option requires both an address and a block identifier.
  - You may only specifiy an address when using the --belongs option.
  - The two --pin_ options, the --clean option, and the --check option are available only in manifest mode.
```

**Source code**: [`internal/chunks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

