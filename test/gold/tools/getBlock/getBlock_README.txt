getBlock argc: 2 [1:-th] 
getBlock -th 
#### Usage

`Usage:`    getBlock [-s|-c|-a|-u|-n|-o|-f|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | blocks | a space-separated list of one or more blocks to retrieve (required) |
| -s | --hashes_only | display only transaction hashes, default is to display full transaction detail |
| -c | --check | compare results between qblocks and Ethereum node, report differences, if any |
| -a | --addrs | display all addresses included in the block |
| -u | --uniq | display only uniq addresses found per block |
| -n | --uniq_tx | display only uniq addresses found per transaction |
| -o | --count_only | display counts of appearances (for --addrs, --uniq, or --uniq_tx only) |
| -f | --filter <addr> | useful only for --addrs or --uniq, only display this address in results |

#### Hidden options (shown during testing only)
| -r | --report | display the latest blocks at both the node and the cache |
| -e | --force | force a re-write of the block to the cache |
| -q | --quiet | do not print results to screen, used for speed testing and data checking |
| -C | --cache | use the cache for data retrieval |
| -m | --normalize | normalize (remove un-common fields and sort) for comparison with other results (testing) |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

