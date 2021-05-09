## [{NAME}]

The `[{NAME}]` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `[{NAME}]` queries the full transactional details of the block (including receipts) for all transactions. You may optionally retreive only the transaction hashes in the block (which is signifcantly faster). You may also use this tool to retrieve uncle blocks at a give height.

Another very useful feature of `[{NAME}]` is the ability to extract all address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This data proves to be very insightful into end user behaviour and adoption analysis.

[{USAGE_TABLE}]
[{FOOTER}]
[{SOURCE}]
