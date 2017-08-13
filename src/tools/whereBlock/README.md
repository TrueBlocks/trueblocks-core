## whereBlock

`whereBlock` looks for and reports on the location of a block. It will return either `cache`, `node`, or `remote` depending if it finds the block in its own cache, had to go to the node to get the block, or had to go remote (because there is no node) to get the block.

If `whereBlock` return `remote` it went to Infura.
#### Usage

`Usage:`    whereBlock [-a|-b|-h]  
`Purpose:`  Reports if a block was found in the cache.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -a | --alone | show the path alone |
| -b | --block | which block to find |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

