## whenBlock

The	`whenBlock` command line tool answers one of two questions: (1) "At what date and time did a given block occur?", and (2) "What block occurred at or before the given date and time?"

In the first case, one simply supplies a block number and that block's date and time is display. In the later case one provides a JSON formatted UTC date (and optionally time) and the block number at or prior to date and time is displayed.

The hour/minute/second portions of the date are optional, and if omitted, default to zero in each case.

#### Help Screen

`Usage:`    whenBlock [-l|-h] date/block  
`Purpose:`  Finds the nearest block before the JSON-formatted date, or the date of the given block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date/block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or blockNumber |
| -l | --list | list special blocks timestamps and dates |
| -h | --help | display this help screen |

[0m	Special values: [first|latest|iceage|homestead|daofund|daohack|daofork|tangerine|spurious|stateclear]

[1;34m  Powered by QuickBlocks.io

#### The --file option

All `quickBlocks` tools support the `--file:filename` command line option. You may place valid command lines for the tool in a file with each command separated by a newline. In some cases this will significantly speed up operation as the tool's data need be loaded only at program start. Lines starting with ';' in these files are comments.
