## cmdLineTest

Testing only

#### Usage

`Usage:`    cmdLineTest [-b|-i|-u|-s|-r|-l|-v|-h] testNum  
`Purpose:`  Tests various command line behaviour.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | testNum | the number of the test to run |
| -b | --bool val | enter a boolean value (either `0`, `1`, `false`, or `true`) |
| -i | --int val | enter any numeric value |
| -u | --uint val | enter any any numeric value greater than or equal to zero |
| -s | --string val | enter any value |
| -r | --range range | enter a range of numeric values |
| -l | --list list | enter a list of value separated by commas (no spaces or quoted) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

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


