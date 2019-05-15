getBalance argc: 2 [1:-th] 
getBalance -th 
#### Usage

`Usage:`    getBalance [-n|-c|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -n | --nozero | suppress the display of zero balance accounts |
| -c | --changes | only report a balance when it changes from one block to the next |

#### Hidden options (shown during testing only)
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

