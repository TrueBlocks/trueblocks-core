## isContract


#### Help Screen

`Usage:`    isContract [-d|-n|-t|-v|-h] addr[s]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) hold byte code (or displays the code).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr[s] | a list of Ethereum addresses from which to check for byte code |
| -d | --display | display the actual code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
| -t | --test | generate intermediary files but do not execute the commands |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Please a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
