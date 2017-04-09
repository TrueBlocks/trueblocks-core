## whenBlock

The	`whenBlock` command line tool answers one of two questions: (1) "At what date and time did a given block occur?", and (2) "What block occurred at or before the given date and time?"

In the first case, one simply supplies a block number and that block's date and time is display. In the later case one provides a JSON formatted UTC date (and optionally time) and the block number at or prior to date and time is displayed.

The hour/minute/second portions of the date are optional, and if omitted, default to zero in each case.

#### Help Screen

`Usage:`    whenBlock [-h] date/block  
`Purpose:`  Finds the nearest block before the JSON-formatted date, or the date of the given block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date/block | YYYY-MM-DD-[HH[:MM[:SS]]], blockNumber, or a special value from [first&#124;latest&#124;iceage&#124;homestead&#124;daohack&#124;daofork&#124;tangerine&#124;spurious] |
| -h | --help | display this help screen |

  Powered by QuickBlocks.io

#### The --file option

All `quickBlocks` tools support the `--file:filename` command line option. You may place valid command lines for the tool in a file with each command separated by a newline. In some cases this will significantly speed up operation as the tool's data need be loaded only at program start. Lines starting with ';' in these files are comments.
