## chifra status


```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
	One or more of [ index | blooms | blocks | transactions | traces | logs | statements | results | state | tokens | monitors | names | abis | slurps | staging | unripe | maps | some | all ]

Flags:
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 10000)
  -a, --chains              include a list of chain configurations in the output
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose output
  -h, --help                display this help screen

Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
```

Data models produced by this tool:

- [status](/data-model/admin/#status)
- [cacheitem](/data-model/admin/#cacheitem)
- [chain](/data-model/admin/#chain)

Links:

- [api docs](/api/#operation/admin-status)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/status)

