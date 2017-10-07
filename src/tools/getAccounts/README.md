## getAccounts

*Published: Saturday, Sept. 29, 2017 - 12:03 am*

`getAccounts` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [ethName](../ethName/README.md).

One way to use this tool is to feed its output through the [getBalance](../getBalance/README.md) or [getTokenBal](../getTokenBal/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    getAccounts

List balances of those accounts (note 'xargs' puts the results on a single line):

    getBalance `getAccounts | xargs`
    
List balances of the local node's accounts in US dollars:

    getBalance `getAccounts | xargs` --dollars

Using ethName to find Singular's address, list token balances held by your accounts:

    getTokenBal `ethName -a singular` `getAccounts | xargs`

Using ethName to find Singular's address, list tokens held by other token accounts:

    getTokenBal `ethName -a singular` `getAccounts -n | xargs`

#### Usage

`Usage:`    getAccounts [-n|-v|-h]  
`Purpose:`  Show the list of Ethereum accounts known to the local node or named accounts.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -n | --named | Show addresses from named accounts as per ethName |
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


