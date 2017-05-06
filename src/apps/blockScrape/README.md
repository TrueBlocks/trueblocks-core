## blockScrape

The `blockScrape` program, using your local node's RPC or IPC, reads each block from any EVM-based blockchain and stores it for later retrieval. After extensive optimizations to the data, including determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized binary database for fast retrieval. By doing as much work as possible before storage, we see significant increase in the speed of retrieval.

If you automate `blockScrape` using something like `cron` it will keep the binary database fresh as per the blockchain its scraping. A sister program, called `miniBlocks` uses the data created by `blockScrape` to build an ultra-light-weight less-detailed, but much faster version of the data.

### Usage

`Usage:`    blockScrape [-c|-l|-f|-r|-d|-e|-n|-i|-v|-h] mode  
`Purpose:`  Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | mode | one of 'check,' 'list,' 'freshen,' or 'refreshen' |
| -c | --check | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -l | --list | list all non-empty block numbers |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -r | --refreshen | remove the full-block index and re-create it |
| -d | --deep | when doing 'check', do a deep check |
| -e | --emptyOnly | when doing 'check', skip non-empty blocks (i.e. check only blocks with no transactions) |
| -n | --noEmpty | when doing 'check', skip empty blocks (i.e. check only blocks with transactions) |
| -i | --indexOnly | only create the index, don't save any blocks (applies only to --refreshen mode) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
