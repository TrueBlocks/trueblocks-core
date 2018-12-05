blockMan argc: 2 [1:-th] 
blockMan -th 
#### Usage

`Usage:`    blockMan [-l|-c|-s|-e|-f|-r|-p|-v|-h]  
`Purpose:`  Show stats or check the block cache.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -l | --list | list all blocks in the index (non-empty blocks only by default) |
| -c | --check | verify the block index (both empty and non-empty by default) |
| -s | --stats | show statistics for the block index (the default) |
| -e | --empty | check or list only empty blocks |
| -f | --full | check or list only full blocks |
| -r | --start val | optionally start at block 'num' |
| -p | --stop val | optionally stop at block 'num' |

#### Hidden options (shown during testing only)
| -k | --skip val | export once every 5,000 lines |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

