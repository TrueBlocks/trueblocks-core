getBloom argc: 2 [1:-th] 
getBloom -th 
#### Usage

`Usage:`    getBloom [-r|-e|-a|-s|-p|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns bloom filter(s) from running node (the default) or as EAB from local cache.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks for which to retrieve blooms |
| -r | --raw | pull the bloom filter directly from the running node (the default) |
| -e | --eab | pull the enhanced adaptive bloom filters (EAB) from the local cache |
| -a | --asbits | display the bloom filter as bits instead of hex |
| -s | --asbars | display the bloom filter as bar charts instead of hex |
| -p | --receipt | display blooms from the transaction receipts as opposed to block (--raw only) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

