## grabABI


#### Usage

`Usage:`    grabABI [-c|-g|-e|-n|-o|-t|-v|-h] addr  
`Purpose:`  Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr | the address(es) of the smart contract(s) to grab |
| -c | --canoncial | convert all types to their canoncial represenation and remove all spaces from display |
| -g | --generate | generate C++ code into ':dir' for all functions and events found in the ABI |
| -e | --encode | generate the encodings for the functions / events in the ABI |
| -n | --noconst | generate encodings for non-constant functions and events only (always true when generating) |
| -o | --open | open the ABI file for editing, download if not already present |
| -t | --test | generate intermediary files but do not execute the commands |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
