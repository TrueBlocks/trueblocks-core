getBalance argc: 2 [1:-th] 
getBalance -th 
#### Usage

`Usage:`    getBalance [-l fn|-n|-d|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -l | --list fn | an alternative way to specify an address_list; place one address per line in the file 'fn' |
| -n | --noZero | suppress the display of zero balance accounts |
| -d | --data | render results as tab delimited data |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `Addresses` must start with '0x' and be forty characters long.
- `Block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- If the queried node does not store historical state, the results are undefined.
- `Special` blocks are detailed under `whenBlock --list`.

