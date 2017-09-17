## getBloom

`getBloom` retreives the bloom filter from a transaction or block.

#### Usage

`Usage:`    getBloom [-e|-r|-q|-v|-h] num  
`Purpose:`  Returns bloom filter(s) from local cache (the default) or directly from a running node.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | num | which bloom (or blooms if more than one) to retreive (or [start-stop) range) |
| -e | --enhanced | retrieve the enhanced bloom filter from the quickBlocks cache |
| -r | --raw | retrieve the bloom filter directly from the running node (includes tx blooms) |
| -q | --quiet | do not print results to screen (useful for performance measurements) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


