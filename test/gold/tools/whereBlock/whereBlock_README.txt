whereBlock argc: 2 [1:-th] 
whereBlock -th 
### Usage

`Usage:`    chifra where [-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Reports in which cache (if any) a block is found.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | blocks | a space-separated list of one or more blocks to search for (required) |
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Customize the location of the cache in the configuration file $CONFIG/trueBlocks.toml.

