getAccounts argc: 2 [1:-th] 
getAccounts -th 
#### Usage

`Usage:`    getAccounts [-e|-m|-o|-c|-p|-n|-a|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses and/or names well known accounts.
        
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | terms | a space separated list of one or more search terms (required) |
| -e | --expand | expand search to include all fields (default searches name, address, and symbol only) |
| -m | --matchCase | do case-sensitive search |
| -o | --owned | Include personal accounts in the search |
| -c | --custom | Include your custom named accounts |
| -p | --prefund | Include prefund accounts |
| -n | --named | Include well know token and airdrop addresses in the search |
| -a | --addr | display only addresses in the results (useful for scripting) |

#### Hidden options (shown during testing only)
| -t | --other | export other addresses if found |
| -x | --fmt <fmt> | export format (one of [none&#124;json&#124;txt&#124;csv&#124;api]) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- With a single search term, the tool searches both `name` and `address`.
- With two search terms, the first term must match the `address` field, and the second term must match the `name` field.
- When there are two search terms, both must match.
- The `--matchCase` option requires case sensitive matching. It works with all other options.
- To customize the list of names add a `custom` section to the config file (see documentation).
- Name file: `~/.quickBlocks/names/names.txt` (195222)

