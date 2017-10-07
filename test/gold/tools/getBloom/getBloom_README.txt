getBloom argc: 2 [1:-th] 
getBloom -th 
#### Usage

`Usage:`    getBloom [-e|-r|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns bloom filter(s) from local cache (the default) or directly from a running node.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a list of one or more blocks at which to report balances, defaults to 'latest' |
| -e | --enhanced | retrieve the enhanced bloom filter from the quickBlocks cache |
| -r | --raw | retrieve the bloom filter directly from the running node (includes tx blooms) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination
- this tool retrieves information from the local node or the ${FALLBACK} node, if configured (see the documentation)
- `special` blocks are detailed under `whenBlock --list`

