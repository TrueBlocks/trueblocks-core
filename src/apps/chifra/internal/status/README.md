## chifra status

The `chifra status` tool reports on the state (and size) of the various TrueBlocks local binary
caches. TrueBlocks produces nine difference caches: `abis`, `blocks`, `monitors`, `names`, `objs`,
`recons`, `slurps`, `traces`, `transactions`. In general practice, these caches may take up a
few GB of hard drive space, however, for very popular smart contract the size of the caches may
grow rather large. Keep an eye on it.

The `chifra status` program allows you to manage the various TrueBlocks caches. You may list all of the
caches, some of the cache, or even individual caches either in terse or full detail. The cache of
interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as
well as other data including blockchain data, and address monitors.

```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
    One or more of [ index | blooms | blocks | transactions | traces | logs | statements | transfers | results | state | tokens | monitors | names | abis | slurps | staging | unripe | maps | some | all ]

Flags:
  -d, --diagnose            same as the default but with additional diagnostics
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 10000)
  -a, --chains              include a list of chain configurations in the output
  -k, --healthcheck         an alias for the diagnose endpoint
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
```

Data models produced by this tool:

- [cacheitem](/data-model/admin/#cacheitem)
- [chain](/data-model/admin/#chain)
- [status](/data-model/admin/#status)

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
