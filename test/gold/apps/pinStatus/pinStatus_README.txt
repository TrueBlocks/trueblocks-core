pinStatus argc: 2 [1:-th] 
pinStatus -th 
#### Usage

`Usage:`    pinStatus [-a|-p|-u|-v|-h] mode  
`Purpose:`  Report on and manage pinned appearance index and bloom chunks.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode | display local or remote manifest or its IPFS hash, one of [local*&#124;remote&#124;onchain] (required) |
| -a | --hash | display the hash instead of contents of manifest ('on' for onchain mode) |
| -p | --pin <str> | pin indexes and blooms, add to manifest, and return hash |
| -u | --unpin <str> | unpin index(es) and blooms given a hash, a filename, or 'all' |
|####|Hidden options||
| -i | --license | show the current pinata license information, if any |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

