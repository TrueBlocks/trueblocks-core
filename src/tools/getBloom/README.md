## getBloom

`getBloom` retreives the bloom filter from the given block or blocks. Alternatively, you may also retrieve the bloom filters from the underlying transaction receipts that make up the block-level bloom. This tool is useful to debug QuickBlocks [monitors](../../monitors) among other things.

#### Usage

`Usage:`    getBloom [-r|-e|-e|-c|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns bloom filter(s) from local cache or directly from a running node.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks from which to retrieve blooms |
| -r | --raw | pull the bloom filter from the running Ethereum node (no cache) |
| -e | --enhanced | retrieve the enhanced bloom filter for a given block (see documentation) |
| -e | --receipts | display receipt level blooms, default is to display only block-level blooms |
| -c | --check | compare results between qblocks and Ethereum node, report differences, if any |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

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


