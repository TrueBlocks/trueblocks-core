
`Usage:`    cacheMan [-b|-c|-f|-l|-n|-e|-m|-t|-v|-h] filenames  
`Purpose:`  Show the contents of an account cache and/or fix it by removing duplicate records.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | filenames | path(s) of files to check, merge, fix or display (default=display) |
| -b | --blockOnly | in 'list' mode, render block number only (used for testing) |
| -c | --check | check for duplicates in the cache |
| -f | --fix | remove duplicates from the cache (if any) |
| -l | --list | list the contents of the cache (the default if no other option) |
| -n | --nocolor | do not color the output |
| -e | --extract | extract transactions for the given contract :id |
| -m | --merge | merge two or more caches into a single cache |
| -t | --truncate | truncate the cache at block :n (implies --fix) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

