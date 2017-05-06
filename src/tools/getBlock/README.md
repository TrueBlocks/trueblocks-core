## getBlock


#### Usage

`Usage:`    getBlock [-f|-m|-b|-p|-t|-s|-v|-h] num  
`Purpose:`  Returns block(s) from local cache (default) or directly from node.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | num | which block (or blocks if more than one) to retreive (or use [start-stop) for range) |
| -f | --fromNode | retreive the block(s) directly from the running node with no processing |
| -m | --mini | output only data fields that exist in miniBlocks and miniTrans |
| -b | --binary | retrieve only quickBlocks binary blocks if they exist, retrieve nothing otherwise |
| -p | --parity | mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers) |
| -t | --terse | retreive transaction hashes instead of full transactions |
| -s | --silent | do not print results to screen, used for speed testing |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
