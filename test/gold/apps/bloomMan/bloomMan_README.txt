bloomMan argc: 2 [1:-th] 
bloomMan -th 
#### Usage

`Usage:`    bloomMan [-s|-r|-c|-u|-b|-u|-v|-h] lists  
`Purpose:`  Work with EABs, raw blooms and/or present statistics.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | lists | list of block_nums or block_nums and addrs (if both, show hit stats for addrs) |
| -s | --stats | calculate statistics for blooms in the block range |
| -r | --rewrite | re-write the given bloom(s) -- works only with block numbers |
| -c | --check | check that a bloom contains all addresses in the given block(s) |
| -u | --upgrade | read, then write, all blooms (effectivly upgrading them) |
| -b | --bucket val | for --stats only, optional bucket size |
| -u | --cum | for --stats only, stats are cummulative (per bucket otherwise) |

#### Hidden options (shown during testing only)
| -r | --raw | print blooms from the raw node |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

