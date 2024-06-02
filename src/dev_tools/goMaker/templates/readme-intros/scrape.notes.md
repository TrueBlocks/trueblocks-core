### configuration

Each of the following additional configurable command line options are available.

**Configuration file:** `trueBlocks.toml`  
**Configuration group:** `[scrape.<chain>]`

| Item         | Type   | Default | Description / Default                                                                                                    |
| ------------ | ------ | ------- | ------------------------------------------------------------------------------------------------------------------------ |
| appsPerChunk | uint64 | 2000000 | the number of appearances to build into a chunk before consolidating it                                                  |
| snapToGrid   | blknum | 250000  | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| firstSnap    | blknum | 2000000 | the first block at which snap_to_grid is enabled                                                                         |
| unripeDist   | blknum | 28      | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe                |
| channelCount | uint64 | 20      | number of concurrent processing channels                                                                                 |
| allowMissing | bool   | false   | do not report errors for blockchains that contain blocks with zero addresses                                             |

Note that for Ethereum mainnet, the default values for appsPerChunk and firstSnap are 2,000,000 and 2,300,000 respectively. See the specification for a justification of these values.

These items may be set in three ways, each overriding the preceding method:

-- in the above configuration file under the `[scrape.<chain>]` group,  
-- in the environment by exporting the configuration item as upper case (with underbars removed) and prepended with (TB underbar SCRAPE underbar CHAIN) with the underbars included, or  
-- on the command line using the configuration item with leading dashes and in snake case (i.e., `--snake_case`).

### further information

Each time `chifra {{.Route}}` runs, it begins at the last block it completed processing (plus one). With
each pass, the scraper descends as deeply as is possible into each block's data. (This is why
TrueBlocks requires a `--tracing` node.) As the scraper encounters appearances of address in the
block's data, it adds those appearances to a growing index. Periodically (after processing the
block that contains the 2,000,000th appearance), the system consolidates an **index chunk**.

An **index chunk** is a portion of the index containing approximately 2,000,000 records (although,
this number is adjustable for different chains). As part of the consolidation, the scraper creates
a Bloom filter representing the set membership in the associated index portion. The Bloom filters
are an order of magnitude smaller than the index chunks. The system then pushes both the index
chunk and the Bloom filter to IPFS. In this way, TrueBlocks creates an immutable, uncapturable
index of appearances that can be used not only by TrueBlocks, but any member of the community who
needs it. (Hint: We all need it.)

Users of the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer) (or any other
software) may subsequently download the Bloom filters, query them to determine which **index
chunks** need to be downloaded, and thereby build a historical list of transactions for a given
address. This is accomplished while imposing a minimum amount of resource requirement on the end
user's machine.

Recently, we enabled the ability for the end user to pin these downloaded index chunks and blooms
on their own machines. The user needs the data for the software to operate--sharing requires
minimal effort and makes the data available to other people. Everyone is better off. A
naturally-occuring network effect.

### tracing

The `scrape` command requires your node to enable the `trace_block` (and related) RPC endpoints. Please see the README file for the `chifra traces` command for more information.

### prerequisites

`chifra {{.Route}}` works with any EVM-based blockchain, but does not currently work without a "tracing,
archive" RPC endpoint. The Erigon blockchain node, given its minimal disc footprint for an archive
node and its support of the required `trace_` endpoint routines, is highly recommended.

Please [see this article](https://trueblocks.io/blog/a-long-winded-explanation-of-trueblocks/) for
more information about running the scraper and building and sharing the index of appearances.
