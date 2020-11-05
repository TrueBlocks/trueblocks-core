## turboDive

`turboDive` is a simple tool that dives into the details of the TurboGeth database. It is not really ready for prime time, so use with caution

#### Usage

`Usage:`    turboDive [-m|-d|-n|-g|-z|-v|-h]  
`Purpose:`  Dive deeply into the turboGeth database.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -m | --mode <val> | the command to run, one of [tables*&#124;dump] |
| -d | --datadir <str> | folder containing TurboGeth data file (data.mdb) |
| -n | --name <str> | for 'dump' command only, the name of the table to dump |
| -g | --goerli | run against the goerli testnet |
| -z | --no_zero | suppress the display of tables with zero records |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by TrueBlocks<sup>&trade;</sup>**


