## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block
occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed.
In the later case, supply a date (and optionally a time) and the block number that occurred at or
just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are
optional, and if omitted, default to zero in each case. Block numbers may be specified as either
integers or hexadecimal number or block hashes. You may specify any number of dates and/or blocks
per invocation.

```[plaintext]
Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.

Usage:
  chifra when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes)

Flags:
  -l, --list         export a list of the 'special' blocks
  -t, --timestamps   display or process timestamps
  -U, --count        with --timestamps only, returns the number of timestamps in the cache
  -r, --repair       with --timestamps only, repairs block(s) in the block range by re-querying from the chain
  -c, --check        with --timestamps only, checks the validity of the timestamp data
  -u, --update       with --timestamps only, bring the timestamp database forward to the latest block
  -d, --deep         with --timestamps --check only, verifies timestamps from on chain (slow)
  -o, --cache        force the results of the query into the cache
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Block numbers, timestamps, or dates in the future are estimated with 13 second blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].
```

Data models produced by this tool:

- [block](/data-model/chaindata/#block)
- [namedblock](/data-model/chaindata/#namedblock)
- [timestamp](/data-model/chaindata/#timestamp)
- [timestampcount](/data-model/chaindata/#timestampcount)

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
