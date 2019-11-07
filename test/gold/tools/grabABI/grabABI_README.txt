grabABI argc: 2 [1:-th] 
grabABI -th 
#### Usage

`Usage:`    grabABI [-c|-g|-n|-s|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | list of one or more smart contracts whose ABI to grab from EtherScan (required) |
| -c | --canonical | convert all types to their canonical represenation and remove all spaces from display |
| -g | --generate | generate C++ code into the current folder for all functions and events found in the ABI |
| -n | --noconst | generate encodings for non-constant functions and events only (always true when generating) |
| -s | --sol <path> | create the ABI file from a .sol file in the local directory |

#### Hidden options (shown during testing only)
| -k | --known | load common 'known' ABIs from cache |
#### Hidden options (shown during testing only)

| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

