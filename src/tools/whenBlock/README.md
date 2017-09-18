## whenBlock

The `whenBlock` tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, one simply supplies a block number and the date and time of that block is displayed. In the later case, one provides a date (and optionally a time) and the block number that occurred at or just prior to that date and time is displayed.

The values for `date` and `time` are specified in JSON format. The `hour`/`minute`/`second` portions of the date are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexidecimal values. You may specify any number of dates or blocks per invocation.

You may name your own blocks (for example, to denote the start of an ICO perhaps) by editing '~/.quickBlocks/quickBlocks.toml' and placing `name=blockNum` pairs under the `[special]` section of that file (create the file if it is not already present). Special blocks are detailed below.

#### Usage

`Usage:`    whenBlock [-a|-l|-v|-h] date / block  
`Purpose:`  Finds the nearest block prior to a JSON-formatted date, or the nearest date prior to
            the given block. Alternatively, search for one of the special blocks listed below.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | date / block | one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber |
| -a | --alone | show the found block or found date unadorned (useful for scripting) |
| -l | --list | list the names and block numbers of special blocks |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

  Notes:
	You may specify any of the following strings to represent 'special' blocks:

	    first (0), iceage (200000), homestead (1150000), daofund (1428756), 
	    daohack (1718497), daofork (1920000), tangerine (2463000), spurious (2675000), 
	    stateclear (2718436), latest (4286085)

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


