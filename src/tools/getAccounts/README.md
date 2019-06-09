## getAccounts

`getAccounts` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [ethName](../ethName/README.md).

One way to use this tool is to feed its output through the [getState](../getState/README.md) or [getTokenInfo](../getTokenInfo/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    getAccounts

List balances of those accounts (note 'xargs' puts the results on a single line):

    getState --mode balance `getAccounts -a | xargs`
    
List balances of the local node's accounts in US dollars:

    getState --mode balance `getAccounts -a | xargs` --dollars

Using ethName to find Singular's address, list token balances held by your accounts:

    getTokenInfo `ethName -a singular` `getAccounts | xargs`

Using ethName to find Singular's address, list tokens held by other token accounts:

    getTokenInfo `ethName -a singular` `getAccounts -n | xargs`

#### Usage

`Usage:`    getAccounts [-o|-c|-p|-n|-a|-v|-h]  
`Purpose:`  List known accounts ('owned' are shown by default).  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -o | --owned | Show owned addresses |
| -c | --custom | Show custom addresses (see below) |
| -p | --prefund | Show prefunded addresses |
| -n | --named | Show named addresses (see ethName) |
| -a | --addr | export only addresses, no names |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- To customize this list add an `custom` section to the config file (see documentation).

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --veryRaw   |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --api_mode  |   simulate api_mode for testing
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


