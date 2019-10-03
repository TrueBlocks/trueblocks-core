ethslurp argc: 2 [1:-th] 
ethslurp -th 
#### Usage

`Usage:`    ethslurp [-t|-b|-s|-x|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetches data from EtherScan for an arbitrary address. Formats the output to your specification.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses to slurp (required) |
|  | block_list | an optional range of blocks to slurp |
| -t | --type enum[ext*|int|token|miner|all] | extract either [ext*&#124;int&#124;token&#124;miner&#124;all] type of transactions |
| -b | --blocks <range> | export records in block range (:0[:max]) |
| -s | --silent | Run silently (only freshen the data, do not display it) |
| -x | --fmt <fmt> | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

