cacheStatus argc: 2 [1:-th] 
cacheStatus -th 
#### Usage

`Usage:`    cacheStatus [-d|-l|-h] mode_list  
`Purpose:`  Report on status of one or more TrueBlocks caches.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode_list | one or more of [scraper&#124;monitors&#124;names&#124;abis&#124;blocks&#124;txs&#124;traces&#124;slurps&#124;prices&#124;some*&#124;all] (required) |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -l | --list | display results in Linux ls -l format (assumes --detail) |

#### Hidden options (shown during testing only)
| -x | --fmt <fmt> | export format (one of [none&#124;json*&#124;txt&#124;csv&#124;api]) |
#### Hidden options (shown during testing only)

| -h | --help | display this help screen |

