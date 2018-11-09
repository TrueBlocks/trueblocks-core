## acctExport

This folder contains a QBlocks monitor. QBlocks monitors pull transactions from the Ethereum blockchain for a given (or a series of) Ethereum addresses.

Below we present the command line interface to this tool, although the tool itself is not available under open source. While the tool is in active development, QBlocks monitors already produce very useful results. For example, we use QBlocks monitors to account for and analyze all transactions on a given smart contract. We present [this example](http://dao.quickblocks.io).

Please contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.

#### Usage

`Usage:`    acctExport [-f|-l|-u|-i|-v|-h]  
`Purpose:`  Export transactions for one or more Ethereum addresses.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -f | --fmt val | export format (one of [json&#124;txt&#124;csv] |
| -l | --filter val | show results for this address (you may specify more than one filter) |
| -u | --useBlooms | use bloom filters to decide whether or not to re-check the cache |
| -i | --ignoreDdos | ignore apparent dDos transactions. |
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


