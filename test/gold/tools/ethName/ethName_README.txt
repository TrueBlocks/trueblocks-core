#### Usage

`Usage:`    ethName [-a|-c|-e|-l|-m|-s|-v|-h] term [name]  
`Purpose:`  Find a name given an Ethereum address, or find an address given a name.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | term [name] | search terms |
| -a | --addrOnly | export only the associated address to be used as input to further commands |
| -c | --count | print only the count of the number of matches |
| -e | --edit | edit the name database |
| -l | --list | list all names in the database |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -s | --source | search 'source' field as well name and address (the default) |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;
