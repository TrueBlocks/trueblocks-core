getBloom argc: 2 [1:-th] 
getBloom -th 
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

