grabABI argc: 2 [1:-th] 
grabABI -th 
### Usage

`Usage:`    chifra abis [-k|-s|-f|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | addrs | list of one or more smart contracts whose ABI to grab from EtherScan (required) |
| -k | --known | load common 'known' ABIs from cache |
| -s | --sol &lt;str&gt; | file name of .sol file from which to create a new known abi (without .sol) |
| -f | --find &lt;str&gt; | try to search for a function declaration given a four byte code |
|####|Hidden options||
| -s | --source | show the source of the ABI information |
| -c | --classes | generate classDefinitions folder and class definitions |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

