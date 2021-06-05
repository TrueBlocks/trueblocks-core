## [{NAME}]

The `[{NAME}]` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `[{NAME}]` queries the full transactional details of the block (including receipts). You may optionally retreive only the transaction hashes in the block (which is signifcantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `[{NAME}]` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behaviour and chain-wide adoption analysis.

[{USAGE_TABLE}]
[{FOOTER}]
[{SOURCE}]
