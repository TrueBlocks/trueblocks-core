getBloom argc: 2 [1:-th] 
getBloom -th 
#### Usage

`Usage:`    getBloom [-r|-e|-b|-c|-a|-i|-n|-v|-h] &lt;block&gt; [block...]  
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
| -n | --bitcount | display the number of bits lit per bloom |

#### Hidden options (shown during testing only)
| -f | --force | force a re-write of the bloom to the cache |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

