cacheStatus argc: 2 [1:-th] 
cacheStatus -th 
### Usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on the status of the TrueBlocks system.

`Where:`  

{{<td>}}
|          | Option                               | Description                                                                                                                                                                               |
| -------- | ------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|          | modes                                | the type of status info to retrieve, one or more of<br/>[index, monitors, collections, names, abis, caches,<br/>some, all]                                                                |
| &#8208;d | &#8208;&#8208;details                | include details about items found in monitors, slurps,<br/>abis, or price caches                                                                                                          |
| &#8208;t | &#8208;&#8208;types &lt;val&gt;      | for caches mode only, which type(s) of cache to report,<br/>one or more of [blocks, transactions, traces, slurps,<br/>prices, all]                                                        |
| ###      | Hidden options                       |                                                                                                                                                                                           |
| &#8208;p | &#8208;&#8208;depth &lt;num&gt;      | for cache mode only, number of levels deep to report                                                                                                                                      |
| &#8208;r | &#8208;&#8208;report                 | show a summary of the current status of TrueBlocks<br/>(deprecated)                                                                                                                       |
| &#8208;e | &#8208;&#8208;terse                  | show a terse summary report                                                                                                                                                               |
| &#8208;m | &#8208;&#8208;migrate &lt;val&gt;    | either effectuate or test to see if a migration is<br/>necessary, one or more of [test, abi_cache, block_cache,<br/>tx_cache, trace_cache, recon_cache, name_cache, slurp_cache,<br/>all] |
| &#8208;g | &#8208;&#8208;get_config             | returns JSON data of the editable configuration file<br/>items                                                                                                                            |
| &#8208;s | &#8208;&#8208;set_config             | accepts JSON in an env variable and writes it to configuration<br/>files                                                                                                                  |
| &#8208;S | &#8208;&#8208;test_start &lt;num&gt; | first block to process (inclusive -- testing only)                                                                                                                                        |
| &#8208;E | &#8208;&#8208;test_end &lt;num&gt;   | last block to process (inclusive -- testing only)                                                                                                                                         |
| ###      | Hidden options                       |                                                                                                                                                                                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;        | export format, one of [none, json, txt, csv, api]                                                                                                                                         |
| &#8208;v | &#8208;&#8208;verbose                | set verbose level (optional level defaults to 1)                                                                                                                                          |
| &#8208;h | &#8208;&#8208;help                   | display this help screen                                                                                                                                                                  |
{{</td>}}

