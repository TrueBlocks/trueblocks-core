## blockScrape

The `blockScrape` program, using your local node's RPC or IPC, reads each block from any EVM-based blockchain and stores it for later retrieval. After extensive optimizations to the data, including determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized binary database for fast retrieval. By doing as much work as possible before storage, we see significant increase in the speed of retrieval.

If you automate `blockScrape` using something like `cron` it will keep the binary database fresh as per the blockchain its scraping. A sister program, called `miniBlocks` uses the data created by `blockScrape` to build an ultra-light-weight less-detailed, but much faster version of the data.

### Usage

`Usage:`    blockScrape [-l|-c|-d|-e|-n|-i|-r|-f|-s|-v|-h]  
`Purpose:`  Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -l | --list | list all non-empty block numbers |
| -c | --check | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -d | --deep | when doing 'check', do a deep check |
| -e | --emptyOnly | when doing 'check', skip non-empty blocks (i.e. check only blocks with no transactions) |
| -n | --noEmpty | when doing 'check', skip empty blocks (i.e. check only blocks with transactions) |
| -i | --indexOnly | only create the index, don't save any blocks (applies only to --refreshen mode) |
| -r | --refreshen | remove the full-block index and re-create it |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -s | --skip | export once every 5,000 lines (applies only to --verbose --list mode - used for testing) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

### Options
