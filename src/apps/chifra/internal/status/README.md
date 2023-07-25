## chifra status


```[plaintext]
Purpose:
  Report on the state of the internal binary caches.

Usage:
  chifra status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
	One or more of [ index | blooms | blocks | txs | traces | monitors | names | abis | recons | slurps | staging | unripe | maps | some | all ]

Flags:
  -c, --first_record uint   the first record to process
  -e, --max_records uint    the maximum number of records to process (default 10000)
  -x, --fmt string          export format, one of [none|json*|txt|csv]
  -v, --verbose             enable verbose (increase detail with --log_level)
  -h, --help                display this help screen

Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
```

Data models produced by this tool:

- [status](/data-model/admin/#status)
- [cacheitem](/data-model/admin/#cacheitem)
- [chain](/data-model/admin/#chain)

<!-- markdownlint-disable MD041 -->
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

