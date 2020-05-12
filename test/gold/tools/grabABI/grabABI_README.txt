grabABI argc: 2 [1:-th] 
grabABI -th 
#### Usage

`Usage:`    grabABI [-c|-g|-n|-m|-k|-a|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | list of one or more smart contracts whose ABI to grab from EtherScan (required) |
| -c | --canonical | convert all types to their canonical represenation and remove all spaces from display |
| -g | --generate | generate C++ code into the current folder for all functions and events found in the ABI |
| -n | --noconst | generate encodings for non-constant functions and events only (always true when generating) |
| -m | --monitored | load ABIs from monitored addresses |
| -k | --known | load common 'known' ABIs from cache |
| -a | --addr | include address of smart contract for the abi in output |

#### Hidden options (shown during testing only)
| -s | --sol <str> | file name of .sol file from which to create a new known abi (without .sol) |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

