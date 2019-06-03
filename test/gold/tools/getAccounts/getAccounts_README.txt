getAccounts argc: 2 [1:-th] 
getAccounts -th 
#### Usage

`Usage:`    getAccounts [-o|-c|-p|-n|-a|-v|-h]  
`Purpose:`  List known accounts ('owned' are shown by default).  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -o | --owned | Show owned addresses |
| -c | --custom | Show custom addresses (see below) |
| -p | --prefund | Show prefunded addresses |
| -n | --named | Show named addresses (see ethName) |
| -a | --addr_only | export only addresses, no names |

#### Hidden options (shown during testing only)
| -x | --fmt val | export format (one of [none&#124;json&#124;txt&#124;csv&#124;api]) |
| -t | --other | export other addresses if found |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- To customize this list add an `custom` section to the config file (see documentation).

