## ethslurp

Drink the blockchain (http://ethslurp.com)

Watch a video tutorial <a href="https://www.youtube.com/watch?v=ZZDV1yAgces">here</a>

Check out Great Hill Corporation on <a href="https://www.facebook.com/GreatHillCorporation/">Facebook</a> or <a href="https://twitter.com/ethslurp">Twitter</a>

### Building from source

Please see the <a href="https://github.com/Great-Hill-Corporation/ethslurp/blob/master/INSTALL.md">INSTALL.md</a> file for more information.

#### Usage

`Usage:`    ethslurp [-a|-b|-d|-n|-r|-f|-i|-e|-o|-l|-v|-h] addr  
`Purpose:`  Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr | the address of the account or contract to slurp |
| -a | --archive val | filename of output (stdout otherwise) |
| -b | --blocks range | export records in block range (:0[:max]) |
| -d | --dates val | export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]]) |
| -n | --name val | name this address |
| -r | --rerun | re-run the most recent slurp |
| -f | --fmt val | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -i | --income | include income transactions only |
| -e | --expense | include expenditures only |
| -o | --open | open the configuration file for editing |
| -l | --list | list previously slurped addresses in cache |
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

Portions of this software are Powered by Etherscan.io APIs
