## chifra scrape

TODO: BOGUS - REVIEW HELP FILES

The `chifra scrape` application creates TrueBlocks' index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. You may start and stop `chifra scrape` as needed, but doing so means the scraper will have to catch up to the front of the chain the next time it runs, a process that may take several hours depending on how long ago it was last run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

You may adjust the speed of the index creation with the `--sleep` and `--block_cnt` options. On some machines, or when running against some EVM node software, you may overburden the hardware. Slowing things down will ensure proper operation. Finally, you may optionally `--pin` each new chunk to IPFS which naturally shards the database among all users.

```[plaintext]
Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.

Usage:
  chifra scrape <mode> [mode...] [flags]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ run | stop ]

Flags:
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -p, --pin              pin chunks (and blooms) to IPFS as they are created (requires pinning service)
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed processing (plus one). With each pass, it descends as deeply as it can into the block's data. (This is why the indexer requires a `--tracing` node.) As addresses appear in the blocks, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records (although, this number is adjustable for different chains). As part of the consolidation, the scraper creates a Bloom filter representing the set membership in the chunk. The Bloom filters are an order of magnitude (or more) smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: We all, every one of us, need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software) may subsequently download the Bloom filters, query them to determine which **chunks** need to be downloaded, and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of resource requirement on the end user's machine.

In future versions of the software, we will pin these downloaded chunks and blooms on end user's machines. The user needs the data for the software to operate and sharing it makes all everyone better off. A naturally-occuring network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software *works* without an `archive` node, but it works significantly better with one. An additional requirement is an RPC that provides OpenEthereum's `trace_` routines. We suggest strongly that you use Erigon for many reasons.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for more information about running the scraper and building and sharing the index of appearances.


#### Other Options

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
