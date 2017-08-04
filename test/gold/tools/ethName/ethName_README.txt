## ethName

`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an equivalent), but in the mean time, `ethname` works well to help you remember who is associated with which addresses.

A very useful way to use of this tools is to feed its output into another command-line tool's input. For example, you could make a command such as:

    ethname -a 'FredJones' | ethscan

which would open the [etscan](http://etherscan.io) website with the address associated with Fred's address(es).

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

#### Notes

With one search term, search in both `name` and `address` fields. With two terms, the first must match the `address` field, while the second must match the `name` field. When there are two search terms, both must match.

##### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;
