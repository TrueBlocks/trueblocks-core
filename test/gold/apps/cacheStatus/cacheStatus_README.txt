cacheStatus argc: 2 [1:-th] 
cacheStatus -th 
#### Usage

`Usage:`    cacheStatus [-d|-l|-r|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on status of one or more TrueBlocks caches.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | modes | which data to retrieve, one or more of [index&#124;monitors&#124;names&#124;abis&#124;blocks&#124;transactions&#124;traces&#124;data&#124;slurps&#124;prices&#124;some*&#124;all] |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -l | --list | display results in Linux ls -l format (assumes --detail) |
| -r | --report | show a summary of the current status of the blockchain and TrueBlocks scrapers |

#### Hidden options (shown during testing only)
| -g | --get_config | returns JSON data of the editable configuration file items |
| -s | --set_config | accepts JSON in an env variable and writes it to configuration files |
| -S | --start <num> | first block to process (inclusive) |
| -E | --end <num> | last block to process (inclusive) |
#### Hidden options (shown during testing only)

|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

