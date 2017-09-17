## [{NAME}]

`whereBlock` reports on the location of a **quickBlocks** block (or blocks). It will return either `cache`, `node`, `remote`, or `none` depending on where it finds the block.

If it finds the block in its own cache, it reports the path to the file in the `cache`. If it finds the block on a locally-running node, it reports `local` followed by the name and version of the locally running node. If the block is not found (that is, the block is not in the cache and there is no locally-running node), it reports 'remote' if the value of the environment variable `$(FALLBACK)` is set and a remote node is found. Otherwise, `whereBlock` reports `none`.

When `whereBlock` reports `remote`, it is reporting on the running node found at the value of the `$(FALLBACK)` environment variable, which currently accepts only a value of `infura`.

This tool is intended mostly as an aid in developing and debugging **quickBlocks** and related tools.

[{USAGE_TABLE}][{FOOTER}]
