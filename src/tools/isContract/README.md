## isContract

This simple program may be used to query an Ethereum address to determine if it is a smart contract or simply a 'regular' account. It may also be used to retrieve the actual byte-code from an address (if code is present). Finally, it may be used to compare two addresses to determine if they hold identical 
code. You may specify multiple addresses on a line.

*See Also*: This command-line tool implements the [eth_getCode](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_getcode) RPC interface.
#### Usage

`Usage:`    isContract [-d|-n|-v|-h] addr[s]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) hold byte code (or displays the code).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr[s] | a list of Ethereum addresses from which to check for byte code |
| -d | --display | display the actual code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

