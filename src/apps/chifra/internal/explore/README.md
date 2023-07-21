## chifra explore

<!-- markdownlint-disable MD041 -->
`chifra explore` opens Etherscan (and other explorers -- including our own) to the block identifier,
transaction identifier, or address you specify. It's a handy (configurable) way to open an explorer
from the command line, nothing more.

```[plaintext]
Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.

Usage:
  chifra explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier

Flags:
  -l, --local    open the local TrueBlocks explorer
  -g, --google   search google excluding popular blockchain explorers
  -o, --cache    force the results of the query into the cache
  -h, --help     display this help screen
```

Data models produced by this tool:

- none

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
  ```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

