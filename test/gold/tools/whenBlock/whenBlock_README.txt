whenBlock argc: 2 [1:-th] 
whenBlock -th 
#### Usage

`Usage:`    whenBlock [-a|-l|-h] date/block  
`Purpose:`  Finds the nearest block before the JSON-formatted date, or the date of the given block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date/block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber |
| -a | --alone | show the found block or found date unadorned (useful for scripting) |
| -l | --list | list special blocks timestamps and dates |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;
