## cacheMan

The `cacheMan` program allows one to  manage and explore [monitor cache files](../../monitors/README.md). You may list cache entries, check for duplicate or invalid cache entries (and optionally remove them), and merge or unmerge one or more caches. Use the tool with caution and please make sure to backup your data before proceeding.

#### Usage

`Usage:`    cacheMan [-c|-d|-f|-l|-s|-e|-t|-m|-v|-h] filenames  
`Purpose:`  Show the contents of an account cache and/or fix it by removing duplicate records.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | filenames | path(s) of files to check, merge, fix or display (default=display) |
| -c | --check | check for duplicates and other problems in the cache |
| -d | --data | in 'list' mode, render results as data (i.e export mode) |
| -f | --fix | remove duplicates from the cache (if any) |
| -l | --list | list the contents of the cache (the default if no other option) |
| -s | --stats | report statistics on the cache |
| -e | --extract val | extract transactions for the given contract :id |
| -t | --truncate val | truncate the cache at block :n (keeps block 'n' and before, implies --fix) |
| -m | --merge | merge two or more caches into a single cache |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


