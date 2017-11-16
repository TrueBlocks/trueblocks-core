isContract argc: 2 [1:-th] 
isContract -th 
#### Usage

`Usage:`    isContract [-d|-b|-n|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) holds byte code (optionally displays the code).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | a space-separated list of one or more Ethereum addresses |
| -d | --data | display results as data (addr <tab> is_contract) |
| -b | --bytes | display the byte code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty characters long.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.

