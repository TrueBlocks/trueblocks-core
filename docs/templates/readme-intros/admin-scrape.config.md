| Item               | Type         | Default      | Description / Default |
| ------------------ | ------------ | ------------ | --------- |
| appsPerChunk       | uint64       | 2000000      | the number of appearances to build into a chunk before consolidating it |
| snapToGrid         | uint64       | 250000       | an override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index |
| firstSnap          | uint64       | 2000000      | the first block at which snap_to_grid is enabled |
| unripeDist         | uint64       | 28           | the distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe |
| channelCount       | uint64       | 20           | number of concurrent processing channels |
| allowMissing       | bool         | false        | do not report errors for blockchains that contain blocks with zero addresses |
