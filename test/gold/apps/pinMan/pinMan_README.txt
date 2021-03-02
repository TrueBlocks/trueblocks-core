pinMan argc: 2 [1:-th] 
pinMan -th 
#### Usage

`Usage:`    pinMan [-a|-p|-u|-i|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on and manage the pinned appearance index chunks and associated bloom filters.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | modes | display the pin manifest (or its hash), using 'modes' as the source, one of [local*&#124;remote&#124;onchain] (required) |
| -a | --hash | display the hash instead of contents of manifest |
| -p | --pin <str> | pin items either locally or remotely ('all' to all items in a folder) |
| -u | --unpin <str> | unpin previously pinned items give a hash, a filename, or 'all' |
| -i | --init | initialize the TrueBlocks appearance index by downloading only bloom filters |
|####|Hidden options||
| -l | --license | show the current pinata license information |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

