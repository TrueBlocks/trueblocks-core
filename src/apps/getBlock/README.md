
`Usage:`    getBlock [-c|-f|-m|-p|-t|-v|-h] num  
`Purpose:`  Returns block(s) from local cache (default) or directly from node.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | num | which block (or blocks if more than one) to retreive (or use [start-stop) for range) |
| -c | --check | pull the given block(s) from both cache and node, compare results, should be no output |
| -f | --fromNode | retreive the block(s) directly from the running node with no processing |
| -m | --mini | output only data fields that exist in miniBlocks and miniTrans |
| -p | --parity | mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers) |
| -t | --terse | retreive transaction hashes instead of full transactions |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

