## getBalance

Retrieve the balance of an Ethereum address (or addresses) at the given block(s). You may specify multiple addresses and/or multiple blocks on the line with at least one address. `block_list`, if not specified, defaults to `latest`.

#### Usage

`Usage:`    getBalance [-n|-d|-v|-h] address_list block_list  
`Purpose:`  Retrieve the balance(s) for one or more accounts at one or more blocks.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | address_list | One or more Ethereum addresses (starting with '0x') from which to retrieve balances |
|  | block_list | a list of one or more blocks at which to report balances, if empty defaults to 'latest' |
| -n | --noZero | suppress the display of zero balance accounts |
| -d | --data | render results as tab delimited data |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- If an address has no ether at a block, the tool reports a zero balance.
- This tool retrieves information from a locally running Ethereum node or the $(FALLBACK) node, if enabled.

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


