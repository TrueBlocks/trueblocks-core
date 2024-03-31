### configuration

Each of the following additional configurable command line options are available.

**Configuration file:** `trueBlocks.toml`  
**Configuration group:** `[scrape.<chain>]`  

| Item         | Type   | Default | Description / Default                                                                                                    |
| ------------ | ------ | ------- | ------------------------------------------------------------------------------------------------------------------------ |
| appsPerChunk | uint64 | 2000000 | the number of appearances to build into a chunk before consolidating it                                                  |
| snapToGrid   | uint64 | 250000  | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| firstSnap    | uint64 | 2000000 | the first block at which snap_to_grid is enabled                                                                         |
| unripeDist   | uint64 | 28      | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe                |
| channelCount | uint64 | 20      | number of concurrent processing channels                                                                                 |
| allowMissing | bool   | false   | do not report errors for blockchains that contain blocks with zero addresses                                             |

Note that for Ethereum mainnet, the default values for appsPerChunk and firstSnap are 2,000,000 and 2,300,000 respectively. See the specification for a justification of these values.

These items may be set in three ways, each overriding the preceding method:

-- in the above configuration file under the `[scrape.<chain>]` group,  
-- in the environment by exporting the configuration item as UPPER&lowbar;CASE (with underbars removed) and prepended with TB_SCRAPE&lowbar;CHAIN&lowbar;, or  
-- on the command line using the configuration item with leading dashes and in snake case (i.e., `--snake_case`).  
