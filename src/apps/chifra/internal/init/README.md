## chifra init

When invoked, `chifra init` reads a value from a smart contract called **The Unchained Index**
(0x0c316b7042b419d07d343f2f4f5bd54ff731183d).

This value (`manifestHashMap`) is an IPFS hash pointing to a pinned file (called the Manifest) that
contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom
filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract.
This makes the index available for our software to use and impossible for us to withhold. Both of
these aspects of the manifest are by design.

If you stop `chifra init` before it finishes, it will pick up again where it left off the next
time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not
previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's
satisfied.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will
start where `init` finished. This means that only the blooms will be stored on your hard drive.
Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete
chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

```[plaintext]
Purpose:
  Initialize the TrueBlocks system by downloading the Unchained Index from IPFS.

Usage:
  chifra init [flags]

Flags:
  -a, --all                in addition to Bloom filters, download full index chunks (recommended)
  -d, --dry_run            display the results of the download without actually downloading
  -F, --first_block uint   do not download any chunks earlier than this block
  -s, --sleep float        seconds to sleep between downloads
  -v, --verbose            enable verbose output
  -h, --help               display this help screen

Notes:
  - If run with no options, this tool will download or freshen only the Bloom filters.
  - The --first_block option will fall back to the start of the containing chunk.
  - You may re-run the tool as often as you wish. It will repair or freshen the index.
```

Data models produced by this tool:

- [chunkrecord](/data-model/admin/#chunkrecord)
- [manifest](/data-model/admin/#manifest)

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
