## getBloom

`getBloom` retreives the bloom filter from a transaction or block.
#### Usage

`Usage:`    getBloom [-s|-f|-p|-r|-t|-q|-v|-h] num  
`Purpose:`  Returns block(s) from local cache (the default) or directly from a running node.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | num | which block (or blocks if more than one) to retreive (or use [start-stop) for range) |
| -s | --source | either :(c)ache or :(r)aw, source for data retrival. (shortcuts -c = qblocks, -r = node) |
| -f | --fields | either :(a)ll, (m)ini, (c)ache or :(r)aw; which fields to include in output (all is default) |
| -p | --parity | mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers) |
| -r | --reciept |  |
| -t | --terse | retreive transaction hashes instead of full transactions |
| -q | --quiet | do not print results to screen, used for speed testing |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;


