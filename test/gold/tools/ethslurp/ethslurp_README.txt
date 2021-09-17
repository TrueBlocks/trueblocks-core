ethslurp argc: 2 [1:-th] 
ethslurp -th 
### Usage

`Usage:`    chifra slurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetch data from EtherScan for any address.

`Where:`  

|     | Option              | Description                                                                                                                |
| --- | ------------------- | -------------------------------------------------------------------------------------------------------------------------- |
|     | addrs               | one or more addresses to slurp from Etherscan (required)                                                                   |
|     | blocks              | an optional range of blocks to slurp                                                                                       |
| -t  | --types &lt;val&gt; | one or more types of transactions to request, one or more of *[ ext\* \| int \| token \| nfts \| miner \| uncles \| all ]* |
| -p  | --appearances       | show only the blocknumer.tx_id appearances of the exported transactions                                                    |
| -x  | --fmt &lt;val&gt;   | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                                                            |
| -v  | --verbose           | set verbose level (optional level defaults to 1)                                                                           |
| -h  | --help              | display this help screen                                                                                                   |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

