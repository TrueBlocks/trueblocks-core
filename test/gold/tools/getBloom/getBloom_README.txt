getBloom argc: 2 [1:-th] 
getBloom -th 
#### Usage

`Usage:`    getBloom [-r|-p|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns bloom filter(s) from local cache or directly from a running node.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks from which to retrieve blooms |
| -r | --raw | pull the bloom filter from the running Ethereum node (no cache) |
| -p | --receipt | display receipt level blooms, default is to display only block-level blooms |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

