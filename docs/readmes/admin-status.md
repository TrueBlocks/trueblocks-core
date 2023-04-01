## chifra status


```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status [flags] <mode>
Arguments:
  mode - the name of the binary cache module to report on
	One of [ index | monitors | names | abis | caches | some | all ]

Flags:
  -t, --types strings   for caches module only, which type(s) of cache to report
                        One or more of [ blocks | txs | traces | slurps | all ]
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
```

Data models produced by this tool:

- none

Links:

- [api docs](/api/#operation/admin-status)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/status)

