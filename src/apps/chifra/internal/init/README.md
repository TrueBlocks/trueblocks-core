## chifra init

<!-- markdownlint-disable MD041 -->
When invoked, `chifra init` reads a value from a smart contract called **The Unchained Index**
([0x0c316b7042b419d07d343f2f4f5bd54ff731183d](https://etherscan.io/address/0x0c316b7042b419d07d343f2f4f5bd54ff731183d)).

This value (`manifestHashMap`) is an IPFS hash pointing to a pinned file (called the Manifest) that
contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom
filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract.
This makes the index available for our software to use and impossible for us to withhold. Both of
these aspects of the manifest are by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next
time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not
previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's
satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will
start where `init` finished. This means that only the blooms will be stored on your hard drive.
Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete
chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all           in addition to Bloom filters, download full index chunks
  -s, --sleep float   seconds to sleep between downloads
  -x, --fmt string    export format, one of [none|json*|txt|csv]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
```

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.*

