## chifra scrape

The `chifra scrape` application creates TrueBlocks' index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as `tmux`. It is possible to start and stop `chifra scrape` as needed, but doing so means the scraper needs to catch up to the front of the chain, a process that may take some time depending on how frequently the scraper is run. See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

The scraper can scrape either the index only, previously created monitors only, both, or neither. If you specify `none`, timestamps will be scraped but nothing else. If you're scraping monitors, you may tell the system to cache traces and transactions. This will speed up access, but take addition hard drive space. You may also adjust the speed of operation on different machines with the `--sleep` and `--block_cnt` options. Finally, you may choose to optionally `--pin` each new chunk to IPFS.

```[plaintext]
Purpose:
  Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.

Usage:
  chifra scrape [flags] [mode...]

Arguments:
  modes - which scraper(s) to control (required)
	One or more of [ indexer | monitors | both ]

Flags:
  -s, --sleep float      seconds to sleep between scraper passes (default 14)
  -p, --pin              pin chunks (and blooms) to IPFS as they are created (requires pinning service)
  -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
  -x, --fmt string       export format, one of [none|json*|txt|csv|api]
  -v, --verbose          enable verbose (increase detail with --log_level)
  -h, --help             display this help screen
```

### explainer

Each time `chifra scrape` runs, it begins at the last block it completed (plus one) and descends as deeply as it can into the block's data. (This is why we need a `--tracing` node.) As address appearances are encountered, the system adds the appearance to a binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the system consolidates a **chunk**.

A **chunk** is a portion of the index containing approximately 2,000,000 records. As part of the consolidation, the scraper creates a Bloom filter representing the chunk. The Bloom filters are an order of magnitude or more smaller than the chunks. The system then pushes both the chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable index of appearances that can be used not only by TrueBlocks, but any member of the community who needs it. (Hint: we all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other software, for that matter) subsequently downloads the Bloom filters, queries them to determine which **chunks** need to be downloaded to the user's machine and thereby build a historical list of transactions for a given address. This is accomplished while imposing a minimum amount of data on the end user's machine.

In future versions of the software, we will pin these shared chunks and blooms on end user's machines. They need the data for the software to operate and sharing it makes all users better off. A naturally-born network effect.

### prerequisites

`chifra scrape` does not work without an RPC endpoint to an Ethereum node. The software works without an `--archive` node, but it works significantly better with one. An additional requirement for the software to work properly is an RPC that provides OpenEthereum's `trace_` routines. We've tested with OpenEthereum and Erigon.

Please see [this article](.) for more information about running the scraper and building and sharing the index of appearances.


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
