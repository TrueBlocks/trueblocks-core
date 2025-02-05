## chifra monitors

`chifra monitors` has two purposes: (1) to display information about the current set of monitors, and (2)
to `--watch` a set of addresses. The `--watch` function allows one to "follow" an address (or set
of addresses) and keep an off-chain database fresh.

### Crud commands

`chifra list` creates a new monitor. See that tool's help file for more information.

The `chifra monitors --delete` command deletes (or `--undelete` if already deleted) an address but does not remove it from your hard drive. The monitor is marked as being deleted, making it invisible to other tools.

Use the `--remove` command to permanently remove a monitor from your computer. This is an
irreversible operation and requires the monitor to have been previously deleted.

The `--decache` option will remove not only the monitor but all of the cached data associated with
the monitor (for example, transactions or traces). This is an irreversible operation (except
for the fact that the cache can be easily re-created with `chifra list <address>`). The monitor need not have been previously deleted.

### Watching addresses

The `--watch` command is special. It starts a long-running process that continually reads the blockchain looking for appearances of the addresses it is instructed to watch. It command requires two additional parameters: `--watchlist <filename>` and `--commands <filename>`. The `--watchlist` file is simply a list of addresses or ENS names, one per line:

```[bash]
0x5e349eca2dc61abcd9dd99ce94d04136151a09ee
trueblocks.eth
0x855b26bc8ebabcdbefe82ee5e9d40d20a1a4c11f
etc.
```

You may monitor as many addresses as you wish, however, if the commands you specify take longer than the `--sleep` amount you specify (14 seconds by default), the results are undefined. (Adjust `--sleep` if necessary.)

The `--commands` file may contain a list of any valid `chifra` command that operates on addresses. (Currently `export`, `list`, `state`, `tokens`.) Each command in the `--commands` file is executed once for each address in the `--watchlist` file. The `--commands` file may contain any number of commands, one per line with the above proviso. For example:

```[bash]
chifra list [{ADDRESS}]
chifra export --logs [{ADDRESS}]
etc.
```

The `[{ADDRESS}]` token is a stand-in for all addresses in the `--watchlist`. Addresses are processed in groups of `batch_size` (default 8).

Invalid commands or invalid addresses are ignored. If a command fails, the process continues with the next command. If a command fails for a particular address, the process continues with the next address. A warning is generated.

```[plaintext]
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process

Flags:
      --delete       delete a monitor, but do not remove it
      --undelete     undelete a previously deleted monitor
      --remove       remove a previously deleted monitor
  -C, --clean        clean (i.e. remove duplicate appearances) from monitors, optionally clear stage
  -l, --list         list monitors in the cache (--verbose for more detail)
  -c, --count        show the number of active monitors (included deleted but not removed monitors)
  -S, --staged       for --clean, --list, and --count options only, include staged monitors
  -D, --decache      removes related items from the cache
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all existing monitors will be cleaned.
```

Data models produced by this tool:

- [message](/data-model/other/#message)
- [monitor](/data-model/accounts/#monitor)
- [monitorclean](/data-model/admin/#monitorclean)

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
