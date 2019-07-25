getState argc: 2 [1:-th] 
getState -th 
#### Usage

`Usage:`    getState [-m|-n|-c|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -m | --mode val | control which state to export. One of [none&#124;some&#124;all&#124;balance&#124;nonce&#124;code&#124;storage&#124;deployed&#124;accttype] |
| -n | --nozero | suppress the display of zero balance accounts |
| -c | --changes | only report a balance when it changes from one block to the next |

#### Hidden options (shown during testing only)
| -o | --noHeader | hide the header in txt and csv mode |
| -x | --fmt val | export format (one of [none&#124;json&#124;txt&#124;csv&#124;api]) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- `block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple modes on a single line.

