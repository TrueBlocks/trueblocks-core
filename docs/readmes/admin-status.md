## chifra status

The `chifra status` program allows you to manage the various TrueBlocks caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called collections.

```[plaintext]
Purpose:
  Report on the status of the TrueBlocks system.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the type of status info to retrieve
	One or more of [ index | monitors | collections | names | abis | caches | some | all ]

Flags:
  -d, --details         include details about items found in monitors, slurps, abis, or price caches
  -t, --types strings   for caches mode only, which type(s) of cache to report
                        One or more of [ blocks | txs | traces | slurps | prices | all ]
  -x, --fmt string      export format, one of [none|json*|txt|csv|api]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
```

**Source code**: [`internal/status`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/status)

