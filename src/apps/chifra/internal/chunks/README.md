## chifra chunks

<!-- markdownlint-disable MD041 -->
The chifra chunks routine provides tools for interacting with, checking the validity of, cleaning up,
and analyizing the Unchained Index. It provides options to list pins, the Manifest, summary data
on the index, Bloom filters, addresses, and appearances. While still in its early stages, this
tool will eventually allow users to clean their local index, clean their remote index, study
the indexes, etc. Stay tuned.

```[plaintext]
Purpose:
  Manage, investigate, and display the Unchained Index.

Usage:
  chifra chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of data to process (required)
	One of [ status | manifest | index | blooms | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges

Flags:
  -c, --check             check the manifest, index, or blooms for internal consistency
  -i, --pin               pin the manifest or each index chunk and bloom
  -p, --publish           publish the manifest to the Unchained Index smart contract
  -n, --truncate uint     truncate the entire index at this block (requires a block identifier)
  -m, --remote            prior to processing, retreive the manifest from the Unchained Index smart contract
  -b, --belongs strings   in index mode only, checks the address(es) for inclusion in the given index chunk
  -s, --sleep float       for --remote pinning only, seconds to sleep between API calls
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose (increase detail with --log_level)
  -h, --help              display this help screen

Notes:
  - Mode determines which type of data to display or process.
  - Certain options are only available in certain modes.
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - The --truncate option updates data, but does not --pin or --publish.
  - You may combine the --pin and --publish options.
  - The --belongs option is only available in the index mode.
```

Data models produced by this tool:

- [manifest](/data-model/admin/#manifest)
- [pinnedchunk](/data-model/admin/#pinnedchunk)
- [chunkindex](/data-model/admin/#chunkindex)
- [chunkblooms](/data-model/admin/#chunkblooms)
- [chunkaddresses](/data-model/admin/#chunkaddresses)
- [chunkappearances](/data-model/admin/#chunkappearances)
- [chunkstats](/data-model/admin/#chunkstats)

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.*

