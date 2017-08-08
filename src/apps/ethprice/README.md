## ethprice

ethprice

Powered, in part, by the Poloniex APIs
#### Usage

`Usage:`    ethprice [-a|-c|-f|-p|-w|-o|-t|-v|-h]  
`Purpose:`  Freshen and/or print price quotes for the Ethereum network.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -a | --at | Report the price since nearest five minutes to :timestamp |
| -c | --clear | Clear the database and re-download price data |
| -f | --freshen | Freshen database even if not needed |
| -p | --period | Time increment for display. Default 120 mins or :t where t is in [5&#124;15&#124;30&#124;120&#124;240&#124;1440] |
| -w | --when | Time of day to start the display. :h determines the hour to start |
| -o | --output | Output level (default 1). :2 exposes more data per record |
| -t | --test | generate intermediary files but do not execute the commands |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

#### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;

