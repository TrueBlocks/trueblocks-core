## blockScrape

***
**Note:** This tool is availble through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

The `blockScrape` app queries your local node (or the ${FALLBACK} node if configured) using the RPC interface reading each block from any EVM-based blockchain. After extensive optimizations to the data, including <img width=500px align="right" src="docs/image.png"> determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized database for fast retrieval. By doing as much work as possible prior to storage, QuickBlocks is able to achieve significant increases in speed of retrieval over the node.


Using operating system tools such as Linux's `cron` you can easily maintain a  constantly fresh QuickBlocks database. Using QuickBlocks `display strings` technology, it is even easy to populate a regular web 2.0 database and from there a full featured website representing the full state of your smart contract.

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
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


w
