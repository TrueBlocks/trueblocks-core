## getBloom

`getBloom` retreives the bloom filter from the given block or blocks. Alternatively, you may also retrieve the bloom filters from the underlying transaction receipts that make up the block-level bloom. This tool is useful to debug QBlocks [monitors](../../monitors) among other things.

#### Usage

`Usage:`    getBloom [-r|-e|-b|-c|-a|-i|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns bloom filter(s) from running node (the default) or as EAB from QBlocks.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks for which to retrieve blooms |
| -r | --raw | pull the bloom filter directly from the running node (the default) |
| -e | --eab | pull the enhanced adaptive blooms from QBlocks cache |
| -b | --block | show only the block-level bloom (--raw only) |
| -c | --receipts | show only the receipt-level blooms (--raw only) |
| -a | --bars | display blooms as bar chart instead of hex |
| -i | --bits | display blooms as bits instead of hex |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

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


