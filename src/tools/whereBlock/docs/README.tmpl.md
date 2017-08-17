## [{NAME}]

`whereBlock` looks for and reports on the location of a block. It will return either `cache`, `node`, or `remote` depending if it finds the block in its own cache, had to go to the node to get the block, or had to go remote (because there is no node) to get the block.

If `whereBlock` return `remote` it went to Infura.

[{COMMAND_HELP}]
