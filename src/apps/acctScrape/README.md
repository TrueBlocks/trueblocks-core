## acctScrape

***
**Note:** This tool is available through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

#### Usage

`Usage:`    acctScrape [-m|-o|-n|-w|-m|-v|-h]  
`Purpose:`  Index transactions for a given Ethereum address (or series of addresses).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -m | --maxBlocks val | the maximum number of blocks to visit during this run |
| -o | --oneBlock val | check if the block would be a hit |
| -n | --oneTrans val | check if the block and transaction would be a hit |
| -w | --writeBlocks | write binary blocks to cache (default: do not write blocks) |
| -m | --maxBlocks val | scan at most --maxBlocks blocks ('all' implies scan to end of chain) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


