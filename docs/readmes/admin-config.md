## chifra config

The `chifra config` program allows you to manage the various TrueBlocks caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, and address monitors.

```[plaintext]
Purpose:
  Report on and edit the configuration of the TrueBlocks system.

Usage:
  chifra config <mode> [mode...] [flags]

Arguments:
  modes - either show or edit the configuration
	One or more of [ show | edit ]

Flags:
      --module strings   the type of information to show or edit
                         One or more of [ index | monitors | names | abis | caches | some | all ]
  -d, --details          include details about items found in monitors, slurps, abis, or price caches
  -t, --types strings    for caches module only, which type(s) of cache to report
                         One or more of [ blocks | txs | traces | slurps | all ]
  -x, --fmt string       export format, one of [none|json*|txt|csv]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

**Source code**: [`internal/config`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/config)

