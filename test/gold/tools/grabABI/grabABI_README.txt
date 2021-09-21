grabABI argc: 2 [1:-th] 
grabABI -th 
### Usage

`Usage:`    chifra abis [-k|-s|-f|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract.

`Where:`

{{<td>}}
|          | Option                         | Description                                                                         |
| -------- | ------------------------------ | ----------------------------------------------------------------------------------- |
|          | addrs                          | list of one or more smart contracts whose ABI to grab<br/>from EtherScan (required) |
| &#8208;k | &#8208;&#8208;known            | load common 'known' ABIs from cache                                                 |
| &#8208;s | &#8208;&#8208;sol &lt;str&gt;  | file name of .sol file from which to create a new known<br/>abi (without .sol)      |
| &#8208;f | &#8208;&#8208;find &lt;str&gt; | try to search for a function declaration given a four<br/>byte code                 |
| ###      | Hidden options                 |                                                                                     |
| &#8208;o | &#8208;&#8208;source           | show the source of the ABI information                                              |
| &#8208;c | &#8208;&#8208;classes          | generate classDefinitions folder and class definitions                              |
| ###      | Hidden options                 |                                                                                     |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;  | export format, one of [none, json, txt, csv, api]                                   |
| &#8208;v | &#8208;&#8208;verbose          | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help             | display this help screen                                                            |
{{</td>}}

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

