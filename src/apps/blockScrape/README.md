## blockScrape

The `blockScrape` program, using your local node's RPC or IPC, reads each block from any EVM-based blockchain and stores it for later retrieval. After extensive optimizations to the data, including determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized binary database for fast retrieval. By doing as much work as possible before storage, we see significant increase in the speed of retrieval.

If you automate `blockScrape` using something like `cron` it will keep the binary database fresh as per the blockchain its scraping. A sister program, called `miniBlocks` uses the data created by `blockScrape` to build an ultra-light-weight less-detailed, but much faster version of the data.

#### Usage

`Usage:`    blockScrape [-c|-l|-f|-r|-d|-e|-n|-i|-v|-h] mode  
`Purpose:`  Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode | one of 'check,' 'list,' 'freshen,' or 'refreshen' |
| -c | --check val | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -l | --list | list all non-empty block numbers |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -r | --refreshen | remove the full-block index and re-create it |
| -d | --deep | when doing 'check', do a deep check |
| -e | --emptyOnly | when doing 'check', skip non-empty blocks (i.e. check only blocks with no transactions) |
| -n | --noEmpty | when doing 'check', skip empty blocks (i.e. check only blocks with transactions) |
| -i | --indexOnly | only create the index, don't save any blocks (applies only to --refreshen mode) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |


#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


