cacheStatus argc: 2 [1:-th] 
cacheStatus -th 
### Usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on the status of the TrueBlocks system.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | modes | the type of status info to retrieve, one or more of *[ index \| monitors \| collections \| names \| abis \| caches \| some\* \| all ]* |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -t | --types &lt;val&gt; | for caches mode only, which type(s) of cache to report, one or more of *[ blocks \| transactions \| traces \| slurps \| prices \| all\* ]* |
|####|Hidden options||
| -p | --depth &lt;num&gt; | for cache mode only, number of levels deep to report |
| -r | --report | show a summary of the current status of TrueBlocks (deprecated) |
| -e | --terse | show a terse summary report |
| -m | --migrate &lt;val&gt; | either effectuate or test to see if a migration is necessary, one or more of *[ test \| abis \| blocks \| txs \| traces \| recons \| all ]* |
| -g | --get_config | returns JSON data of the editable configuration file items |
| -s | --set_config | accepts JSON in an env variable and writes it to configuration files |
| -S | --test_start &lt;num&gt; | first block to process (inclusive -- testing only) |
| -E | --test_end &lt;num&gt; | last block to process (inclusive -- testing only) |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

