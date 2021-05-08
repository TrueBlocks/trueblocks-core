## [{NAME}]

The `[{NAME}]` application creates TrueBlocks' index of address appearances--the fundemental data structure of the entire TrueBlocks system. It also, optionally, pins the index to IPFS.

`[{NAME}]` is a long running process, therefore we advise you run it as a service in your operating system or at least in terminal multiplexer such as `tmux`. It is possible to start and stop `[{NAME}]` as needed, but doing so necessitates that the scraper catches up to the front of the chain. A process that may take some time depending on how recently it's been run.

See below for a more in depth explanation of how the scraping process works and prerequisites for it proper operation.

[{USAGE_TABLE}]

### explainer

Each time `[{NAME}]` runs, it begins at the last block it encountered and decends as deeply into the block's data as is possible (that's why we need a --tracing node). As it encounters address appearances, it add that appearance to a growing binary index. Periodically (at the end of the block containing the 2,000,000th appearance), the process consolidates a **chunk**. A **chunk** is a portion of the index containing approximately 2,000,000 records. The process then lays a bloom filter in front of the chunk and pushes both the chunk and the bloom filter to IPFS. In this way, we create an immutable, uncapturable index appearances not only for our own use but for the use of the entire community.

Users of our software (or any other software using our index) may then subsequently download the bloom filters (which are very small), query them to determine which **chunks** to download and thereby build a list of every historical transaciton for whatever address they are interested in. This can happen while imposing a minimal amount of data on the end user's machine.

### prerequisites

`[{NAME}]` will not work without access to the RPC endpoint of an Ethereum node. It works without an `--archive` node, but works significantly better with one. Support for OpenEthereum `trace_` routines is a hard requirement.

Please see [this article](.) for more information about running this application and building the index of appearances.

[{FOOTER}]
[{SOURCE}]
