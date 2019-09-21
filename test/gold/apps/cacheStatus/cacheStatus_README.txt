cacheStatus argc: 2 [1:-th] 
cacheStatus -th 
#### Usage

`Usage:`    cacheStatus [-d|-l|-v|-h] mode_list  
`Purpose:`  Report on status of one or more TrueBlocks caches.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode_list | one or more of [index&#124;monitors&#124;names&#124;abis&#124;blocks&#124;txs&#124;traces&#124;slurps&#124;prices&#124;some*&#124;all] |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -l | --list | display results in Linux ls -l format (assumes --detail) |

#### Hidden options (shown during testing only)
| -x | --fmt <fmt> | export format (one of [none&#124;json*&#124;txt&#124;csv&#124;api]) |
| -g | --config-get | returns JSON data of the editable configuration file items |
| -s | --config-set | accepts JSON config data and writes it to configuration files |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

