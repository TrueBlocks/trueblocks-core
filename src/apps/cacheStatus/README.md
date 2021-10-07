## chifra status

The `chifra status` program allows you to manage the various TrueBlocks caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called collections.

```[plaintext]
Purpose:
  Report on the status of the TrueBlocks system.

Usage:
  chifra status [flags] [mode...]

Arguments:
  modes - the type of status info to retrieve
	One or more of [ index | monitors | collections | names | abis | caches | some | all ]

Flags:
  -d, --details         include details about items found in monitors, slurps, abis, or price caches
  -t, --types strings   for caches mode only, which type(s) of cache to report
                        One or more of [ blocks | txs | traces | slurps | prices | all ]

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)
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
