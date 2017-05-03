
`Usage:`    miniBlocks [-c|-l|-f|-r|-d|-v|-h] mode  
`Purpose:`  Freshen or check the miniBlock database.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | mode | one of 'check,' 'list,' 'freshen,' or 'refreshen' |
| -c | --check | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -l | --list | list all non-empty block numbers |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -r | --refreshen | remove the full-block index and re-create it |
| -d | --deep | when doing 'check', do a deep check |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

