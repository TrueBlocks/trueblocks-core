whereBlock argc: 2 [1:-th] 
whereBlock -th 
#### Usage

`Usage:`    whereBlock [-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Reports if a block was found in the cache, at a local, or at a remote node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | blocks | a space-separated list of one or more blocks to search for (required) |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- You may customize the location of your cache in the file ~/.quickBlocks/quickBlocks.toml.

