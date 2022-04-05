## chifra monitors

A TrueBlocks monitor is simply a file on your computer that represents the transactional history of a given Ethereum address. Monitors do not exist until you indicate your interest in a certain address. (See `chifra list`.)

You may use the `--delete` command to delete (or undelete if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversible operation.

```[plaintext]

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
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
