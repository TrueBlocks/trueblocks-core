## cacheMan

The `cacheMan` program allows one to  manage and explore [monitor cache files](../../monitors/README.md). You may list cache entries, check for duplicate or invalid cache entries (and optionally remove them), and merge or unmerge one or more caches. Use the tool with caution and please make sure to backup your data before proceeding.

#### Usage

`Usage:`    cacheStatus [-d|-l|-v|-h] mode_list  
`Purpose:`  Report on status of one or more TrueBlocks caches.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode_list | which data to retreive, one or more of [index&#124;monitors&#124;names&#124;abis&#124;blocks&#124;transactions&#124;traces&#124;slurps&#124;prices&#124;some*&#124;all] |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -l | --list | display results in Linux ls -l format (assumes --detail) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


