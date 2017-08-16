## getTrace

`getTrace` returns the traces from the given transaction.
#### Usage

`Usage:`    getTrace [-t|-r|-v|-h] hash  
`Purpose:`  Retrieve the full trace of a transaction.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | hash | transaction hash of the requested transaction |
| -t | --terse | display the traces in a terse fashion |
| -r | --raw | return the trace directly from the node without processing |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

