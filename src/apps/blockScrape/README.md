
`Usage:`    blockScrape [-l|-c|-d|-i|-r|-f|-s|-v|-h]  
`Purpose:`  Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.

`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -l | --list | list all non-empty block numbers |
| -c | --check | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -d | --deep | when doing 'check', do a deep check |
| -i | --indexOnly | only create the index, don't save any blocks (applies only to --refreshen mode) |
| -r | --refreshen | remove the full-block index and re-create it |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -s | --skip | export once every 5,000 lines (applies only to --verbose --list mode - used for testing) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io
