## chifra config

<!-- markdownlint-disable MD041 -->
The `chifra config` program allows you to manage the various TrueBlocks caches. You may list all of the
caches, some of the cache, or even individual caches either in terse or full detail. The cache of
interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as
well as other data including blockchain data, and address monitors.

```[plaintext]
Purpose:
  Report on and edit the configuration of the TrueBlocks system.

Usage:
  chifra config <mode> [flags]

Arguments:
  mode - either show or edit the configuration
	One of [ show | edit ]

Flags:
  -a, --paths        show the configuration paths for the system
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen
```

Data models produced by this tool:

- [chain](/data-model/admin/#chain)

Links:

- [api docs](/api/#operation/admin-config)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/config)

