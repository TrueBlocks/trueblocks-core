getBalance argc: 2 [1:-th] 
getBalance -th 
#### Usage

`Usage:`    getBalance [-d|-e|-v|-h] addrs blocks  
`Purpose:`  Retrieve the balance for an account at a block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addrs | Ethereum address (starting with '0x') from which to retrieve the balance |
|  | blocks | the block at which to retrieve the balance (defaults to 'latest') |
| -d | --data | render results as tab delimited data |
| -e | --ether | return the balance in Ether instead of Wei |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;
