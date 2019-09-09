getBlock argc: 2 [1:-th] 
getBlock -th 
#### Usage

`Usage:`    getBlock [-n|-c|-a|-u|-q|-m|-l|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks to retrieve (required) |
| -n | --hash_only | display only transaction hashes, default is to display full transaction detail |
| -c | --check | compare results between qblocks and Ethereum node, report differences, if any |
| -a | --addrs | display all addresses included in the block |
| -u | --uniq | display only uniq addresses found per block |
| -q | --uniqTx | display only uniq addresses found per transaction |
| -m | --number | display address counts (alterntively --addrCnt, --uniqTxCnt, or --uniqCnt) |
| -l | --filter <address> | useful only for --addrs or --uniq, only display this address in results |

#### Hidden options (shown during testing only)
| -l | --latest | display the latest blocks at both the node and the cache |
| -o | --force | force a re-write of the block to the cache |
| -q | --quiet | do not print results to screen, used for speed testing and data checking |
| -s | --source enum[c*|r] | either :c(a)che or :(r)aw, source for data retrival. (shortcuts -k = qblocks, -r = node) |
| -f | --fields enum[a*|m|c|r] | either :(a)ll, (m)ini, (c)ache or :(r)aw; which fields to include in output (all is default) |
| -n | --normalize | normalize (remove un-common fields and sort) for comparison with other results (testing) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- `special` blocks are detailed under `whenBlock --list`.

