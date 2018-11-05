bloomMan argc: 2 [1:-th] 
bloomMan -th 
#### Usage

`Usage:`    bloomMan [-s|-b|-u|-r|-c|-u|-v|-h] lists  
`Purpose:`  Work with QBlocks-style bloom filters and/or present statistics.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | lists | list of either account address, blocks, or both (if both, show hits statistics) |
| -s | --stats | calculate statistics for blooms in the block range |
| -b | --bucket val | optional bucket size of --stats |
| -u | --cum | statistics are cummulative (per bucket otherwise) |
| -r | --rewrite | re-write the given bloom(s) -- works only with block numbers |
| -c | --check | check that a bloom contains all addresses in the given block(s) |
| -u | --upgrade | read, then write, all blooms (effectivly upgrading them) |

#### Hidden options (shown during testing only)
| -r | --raw | print blooms from the raw node |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

