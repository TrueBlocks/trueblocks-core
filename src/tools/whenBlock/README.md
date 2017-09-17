## whenBlock

The `whenBlock` tool answers one of two questions: (1) "When did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, one simply supplies a block number and the date and time of that block is displayed. In the later case, one provides a date (and optionally time) and the block number that 
occurred at or just prior to that date and time is displayed.

Name your own blocks by editing '~./quickBlocks/whenBlock.toml' and placing `name=blockNum` pairs under the [SPECIAL] section of that file (create it if not present).

The `hour`/`minute`/`second` portions of the date are optional, and if omitted, default to zero in each case.

   Special values: [ iceage | homestead | daofund | daohack | daofork | tangerine | spurious | stateclear | latest ]

#### Usage

`Usage:`    whenBlock [-a|-l|-v|-h] date / block  
`Purpose:`  Finds the nearest block prior to a JSON-formatted date, or the nearest date prior to
           the given block. Alternatively, search for one of the special blocks listed below.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date / block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber |
| -a | --alone | show the found block or found date unadorned (useful for scripting) |
| -l | --list | list special blocks timestamps and dates |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

  Notes:
	You may specify any of the following strings to represent 'special' blocks:

	    first (0), iceage (200000), homestead (1150000), daofund (1428756), 
	    daohack (1718497), daofork (1920000), tangerine (2463000), spurious (2675000), 
	    stateclear (2718436), latest (4281323)

	See the README for information on customizing this list.

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |         Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. Use semi-colon make comments.*

**Powered by QuickBlocks<sup>&reg;</sup>**


