## [{NAME}]

`[{NAME}]` reports on the location of a block (or blocks) in the cache. It reports on one of three situations: `cache`, `node` or `none` depending on where the block is found.

If `[{NAME}]` finds the block in the TrueBlocks cache, it reports the path to that block's file. If the block is not in the cache, but there is a locally running node, `[{NAME}]` returns `node` plus the name and version info from the running node. Otherwise, `[{NAME}]` returns `none`.

This tool is intended mostly as an aid in developing and debugging TrueBlocks tools.

[{USAGE_TABLE}][{FOOTER}]
