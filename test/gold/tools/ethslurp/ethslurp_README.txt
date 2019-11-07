ethslurp argc: 2 [1:-th] 
ethslurp -th 
#### Usage

`Usage:`    ethslurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetches data from EtherScan for an arbitrary address.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses to slurp from Etherscan (required) |
|  | blocks | an optional range of blocks to slurp |
| -t | --type <val> | which types of transaction to request, one or more of [ext*&#124;int&#124;token&#124;miner&#124;all] |
| -p | --appearances | show only the blocknumer.tx_id appearances of the exported transactions |
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

