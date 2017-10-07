getBlock argc: 2 [1:-th] 
getBlock -th 
#### Usage

`Usage:`    getBlock [-k|-s|-f|-p|-t|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache (the default) or directly from a running node.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a list of one or more blocks to retrieve, defaults to 'latest' |
| -k | --check | pull block(s) using both cache and raw, compare results, report differences |
| -s | --source val | either :(c)ache or :(r)aw, source for data retrival. (shortcuts -c = qblocks, -r = node) |
| -f | --fields val | either :(a)ll, (m)ini, (c)ache or :(r)aw; which fields to include in output (all is default) |
| -p | --parity | mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers) |
| -t | --terse | if source is raw, retreive transaction hashes instead of full transactions |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination
- this tool retrieves information from the local node or the ${FALLBACK} node, if configured (see the documentation)
- `special` blocks are detailed under `whenBlock --list`

