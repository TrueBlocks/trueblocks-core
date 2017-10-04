## chifra

`chifra` is an interactive command line tool that builds a QuickBlocks account monitor program through an interactive series of questions. Given answers to these questions (such as which addresses to monitor, what to name those addresses, `chifra` builds both the parsing library needed to monitor the given addresses as well as an interative transactional debugging/accounting program. It does this through the use of [makeClass](../makeClass) and [grabABI](../grabABI).
#### Usage

`Usage:`    chifra [-s|-v|-h] folder addrs  
`Purpose:`  Interactively creates a QuickBlocks project to monitor an Ethereum address.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | folder | name of the folder into which to store the monitor |
|  | addrs | a list of one or more addresses to monitor (must start with '0x') |
| -s | --silent | supress all output from chifra (normally chifra is quite verbose) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |


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


