## whenBlock

The	`whenBlock` tool answers one of two questions: (1) "When did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, one simply supplies a block number and the date and time of that block is displayed. In the later case, one provides a date (and optionally time) and the block number that occurred at or just prior to that date and time is displayed.

The `hour`/`minute`/`second` portions of the date are optional, and if omitted, default to zero in each case.

#### Help Screen

`Usage:`    whenBlock [-l|-h] date/block  
`Purpose:`  Finds the nearest block before the JSON-formatted date, or the date of the given block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date/block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber |
| -l | --list | list special blocks timestamps and dates |
| -h | --help | display this help screen |

  Special values: [`first`|`latest`|`iceage`|`homestead`|`daofund`|`daohack`|`daofork`|`tangerine`|`spurious`|`stateclear`]

#### The --file option

All `quickBlocks` command-line tools support the `--file:filename` command line option. You may place valid command lines, one on each line, for the particular tool in a file. In some cases this will significantly speed up operation as the tool's data need be loaded only at program start. Lines beginning with ';' are interpreted as comments.

Powered by QuickBlocks.io
