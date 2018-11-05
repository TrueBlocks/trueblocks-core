blockMan argc: 2 [1:-th] 
blockMan -th 
#### Usage

`Usage:`    blockMan [-c|-l|-r|-d|-e|-n|-v|-h] mode  
`Purpose:`  Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode | one of 'list,' 'check,' or 'freshhen' (defaults to 'freshen') |
| -c | --check val | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -l | --list | list all non-empty block numbers |
| -r | --refreshen | remove the full-block index and re-create it |
| -d | --deep | when doing 'check', do a deep check |
| -e | --emptyOnly | when doing 'check', skip non-empty blocks (i.e. check only blocks with no transactions) |
| -n | --noEmpty | when doing 'check', skip empty blocks (i.e. check only blocks with transactions) |

#### Hidden options (shown during testing only)
| -s | --skip | export once every 5,000 lines (applies only to --verbose --list mode - used for testing) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

