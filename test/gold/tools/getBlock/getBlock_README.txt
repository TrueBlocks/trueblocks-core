getBlock argc: 2 [1:-th] 
getBlock -th 
#### Usage

`Usage:`    getBlock [-e|-a|-u|-n|-c|-U|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | blocks | a space-separated list of one or more blocks to retrieve (required) |
| -e | --hashes_only | display only transaction hashes, default is to display full transaction detail |
| -a | --apps | display all address appearances included in the block |
| -u | --uniq | display only uniq addresses found per block |
| -n | --uniq_tx | display only uniq addresses found per transaction |
| -c | --count | display counts of appearances (for --apps, --uniq, or --uniq_tx) or transactions |
| -U | --uncles | display uncle blocks (if any) instead of the requested block |
|####|Hidden options||
| -o | --cache | force a re-write of the block to the cache |
| -t | --trace | export the traces from the block as opposed to the block data |
|####|Hidden options||
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `special` blocks are detailed under `whenBlock --list`.

