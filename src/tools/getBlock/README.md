## getBlock

`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an equivalent), but in the mean time, `ethname` works well to help you remember who is associated with which addresses.

A very useful way to use of this tools is to feed its output into another command-line tool's input. For example, you could make a command such as:

    ethname -a 'FredJones' | ethscan

which would open the [etherscan](http://etherscan.io) website with the address associated with Fred's address(es).

#### Notes

With one search term, search in both `name` and `address` fields. With two terms, the first must match the `address` field, while the second must match the `name` field. When there are two search terms, both must match.

#### Usage

`Usage:`    getBlock [-s|-f|-p|-r|-t|-q|-v|-h] num  
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
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

