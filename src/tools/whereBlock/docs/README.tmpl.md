## [{NAME}]

`whereBlock` reports on the location of a block (or blocks) in the cache. It reports on one of four situations: `cache`, `node`, `fallback`, or `none` depending on where the block is found.

If `whereBlock` finds the block in the QuickBlocks cache, it reports the path to that block's file. If the block is not in the cache, but there is a locally running node, `whereBlock` returns `node` plus the name and version info from the running node. Otherwise, `whereBlock` returns the setting of the `${FALLBACK}` envirnment variable (if it's set), or `none` otherwise. The `${FALLBACK}` environment variable currently only accepts a value of `infura`.

This tool is intended mostly as an aid in developing and debugging  QuickBlocks tools.

[{USAGE_TABLE}][{FOOTER}]
