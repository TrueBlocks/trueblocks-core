## chifra scrape

The `chifra scrape` application creates TrueBlocks' index of address appearances--the fundemental data structure of the entire TrueBlocks system. It also, optionally, pins the index to IPFS.

`chifra scrape` is a long running process, therefore we advise you run it as a service in your operating system or at least in terminal multiplexer such as `tmux`. It is possible to start and stop `chifra scrape` as needed, but doing so necessitates that the scraper catches up to the front of the chain. A process that may take some time depending on how recently it's been run.

See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

### usage

`Usage:`    chifra scrape [-t|-n|-p|-s|-i|-R|-v|-h] mode  
`Purpose:`  Decentralized blockchain scraper and block cache.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | mode | control the block and account scrapers, one of *[ run \| quit \| pause \| restart ]* (required) |
| -t | --tool <val> | process the index, monitors, or both (none means process timestamps only), one or more of *[ monitors \| index\* \| none \| both ]* |
| -n | --n_blocks <num> | maximum number of blocks to process (defaults to 5000) |
| -p | --pin | pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node) |
| -s | --sleep <double> | the number of seconds to sleep between passes (default 14) |
| -i | --cache_txs | write transactions to the cache (see notes) |
| -R | --cache_traces | write traces to the cache (see notes) |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

### explainer

Each time `chifra scrape` runs, it begins at the last block it encountered and decends as deeply into the block's data as is possible (that's why we need a --tracing node). As it encounters address appearances, it add that appearance to a growing binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the process consolidates a **chunk**. A **chunk** is a portion of the index containing approximately 2,000,000 records. The process then lays a bloom filter in front of the chunk and pushes both the chunk and the bloom filter to IPFS. In this way, we create an immutable, uncapturable index appearances not only for our own use but for the use of the entire community.

Users of our software (or any other software using our index) may then subsequently download the bloom filters (which are very small), query them to determine which **chunks** to download and thereby build a list of every historical transaciton for whatever address they are interested in. This can happen while imposing a minimal amount of data on the end user's machine.

### prerequisites

`chifra scrape` will not work without access to the RPC endpoint of an Ethereum node. It works without an `--archive` node, but works significantly better with one. Support for OpenEthereum `trace_` routines is a hard requirement.

Please see [this article](.) for more information about running this application and building the index of appearances.


**Source code**: [`apps/blockScrape`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/blockScrape)

