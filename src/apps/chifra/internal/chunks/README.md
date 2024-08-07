## chifra chunks

The `chifra chunks` routine provides tools for interacting with, checking the validity of, cleaning up,
and analyzing the Unchained Index. It provides options to list pins, the Manifest, summary data
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
    One of [ manifest | index | blooms | pins | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges

Flags:
  -c, --check              check the manifest, index, or blooms for internal consistency
  -i, --pin                pin the manifest or each index chunk and bloom
  -p, --publish            publish the manifest to the Unchained Index smart contract
  -r, --remote             prior to processing, retrieve the manifest from the Unchained Index smart contract
  -b, --belongs strings    in index mode only, checks the address(es) for inclusion in the given index chunk
  -F, --first_block uint   first block to process (inclusive)
  -L, --last_block uint    last block to process (inclusive)
  -m, --max_addrs uint     the max number of addresses to process in a given chunk
  -d, --deep               if true, dig more deeply during checking (manifest only)
  -e, --rewrite            for the --pin --deep mode only, writes the manifest back to the index folder (see notes)
  -U, --count              for certain modes only, display the count of records
  -s, --sleep float        for --remote pinning only, seconds to sleep between API calls
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose output
  -h, --help               display this help screen

Notes:
  - Mode determines which type of data to display or process.
  - Certain options are only available in certain modes.
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - The --truncate option updates the manifest and removes local data, but does not alter remote pins.
  - The --belongs option is only available in the index mode.
  - The --first_block and --last_block options apply only to addresses, appearances, and index --belongs mode.
  - The --pin option requires a locally running IPFS node or a pinning service API key.
  - The --publish option requires a private key.
  - The --publisher option is ignored with the --publish option since the sender of the transaction is recorded as the publisher.
  - Without --rewrite, the manifest is written to the temporary cache. With it, the manifest is rewritten to the index folder.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [appearancetable](/data-model/accounts/#appearancetable)
- [chunkaddress](/data-model/admin/#chunkaddress)
- [chunkbloom](/data-model/admin/#chunkbloom)
- [chunkindex](/data-model/admin/#chunkindex)
- [chunkpin](/data-model/admin/#chunkpin)
- [chunkrecord](/data-model/admin/#chunkrecord)
- [chunkstats](/data-model/admin/#chunkstats)
- [count](/data-model/other/#count)
- [ipfspin](/data-model/admin/#ipfspin)
- [manifest](/data-model/admin/#manifest)
- [message](/data-model/other/#message)
- [reportcheck](/data-model/admin/#reportcheck)

### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
