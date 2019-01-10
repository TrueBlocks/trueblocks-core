## blockMan

The `blockMan` app queries the rpcProvider you specify (or your local node if none) using the RPC interface reading each block from any EVM-based blockchain. After extensive optimizations to the data, including 
<img width=500px align="right" src="docs/image.png"> determining each transaction's error status and expanding internal message calls, the blocks are stored in a speed-optimized database for fast retrieval. By doing as much work as possible prior to storage, QBlocks is able to achieve significant increases in speed of retrieval over the node.

Using operating system tools such as Linux's `cron` you can easily maintain a  constantly fresh QBlocks database. Using QBlocks `display strings` technology, it is even easy to populate a regular web 2.0 database and from there a full featured website representing the full state of your smart contract.

#### Usage

`Usage:`    blockMan [-l|-c|-s|-e|-f|-r|-p|-v|-h]  
`Purpose:`  Show stats or check the block cache.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -l | --list | list all blocks in the index (non-empty blocks only by default) |
| -c | --check | verify the block index (both empty and non-empty by default) |
| -s | --stats | show statistics for the block index (the default) |
| -e | --empty | check or list only empty blocks |
| -f | --full | check or list only full blocks |
| -r | --start val | optionally start at block 'num' |
| -p | --stop val | optionally stop at block 'num' |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


