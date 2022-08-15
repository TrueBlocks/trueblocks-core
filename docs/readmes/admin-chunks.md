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
  mode - the type of information to retrieve (required)
	One of [ status | index | blooms | manifest | stats | addresses | appearances ]
  blocks - optional list of blocks used to intersect with chunk ranges
  addrs - optional list of addresses for use with --belongs option (see notes)

Flags:
  -i, --pin          pin all chunks (locally if IPFS daemon is running, and/or remotely with --remote flag)
  -m, --remote       for some options, forces processing to use remote data
  -p, --publish      repin chunks, pin the manifest, and publish to the Unchained Index smart contract
  -c, --check        depends on mode, checks for internal consistency of the given type
  -b, --belongs      in index mode only, checks if the given address appears in the given chunk
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
  - The --repair and --reset options also update the manifest, but do not publish it.
```

**Source code**: [`internal/chunks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/chunks)

