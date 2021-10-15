## chifra chunks

This tool is not yet ready for production use. Please return to this page later.

```[plaintext]
Purpose:
  Manage and investigate chunks and bloom filters.

Usage:
  chifra chunks [flags] <block> [block...]

Arguments:
  blocks - an optional list of blocks to process

Flags:
  -l, --list             list the bloom and index hashes from local cache or IPFS
  -c, --check            check the validity of the chunk or bloom
  -e, --extract string   show the some or all of the contents of the chunk or bloom filters
                         One of [ header | addr_table | app_table | blooms | all ]
  -s, --stats            for the --list option only, display statistics about each chunk or bloom
  -a, --save             for the --extract option only, save the entire chunk to a similarly named file as well as display

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - Only a single block in a given chunk needs to be supplied.
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
