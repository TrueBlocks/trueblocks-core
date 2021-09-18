ethslurp argc: 2 [1:-th] 
ethslurp -th 
### Usage

`Usage:`    chifra slurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetch data from EtherScan for any address.

`Where:`  

{{<td>}}
|          | Option                          | Description                                                                                                  |
| -------- | ------------------------------- | ------------------------------------------------------------------------------------------------------------ |
|          | addrs                           | one or more addresses to slurp from Etherscan (required)                                                     |
|          | blocks                          | an optional range of blocks to slurp                                                                         |
| &#8208;t | &#8208;&#8208;types &lt;val&gt; | one or more types of transactions to request, one or<br/>more of [ext, int, token, nfts, miner, uncles, all] |
| &#8208;p | &#8208;&#8208;appearances       | show only the blocknumer.tx_id appearances of the exported<br/>transactions                                  |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                            |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                             |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                     |
{{</td>}}

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

