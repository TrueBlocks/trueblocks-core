## ethname


#### Help Screen

`Usage:`    ethname [-a|-c|-l|-m|-o|-w|-v|-h] term [name]  
`Purpose:`  Find a name given an Ethereum address, or find an address given a name.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | term [name] | search for 'term' in either name or address. If 'name' is present, term 
			     is assumed to be an address. In this case, both must match |
| -a | --all | search 'source' field as well name and address |
| -c | --count | print only the count of the number of matches |
| -l | --list | list all names |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -o | --open | open the name database for editing |
| -w | --write | write the name/address pair to database. Will prompt for an optional source |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Please a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
