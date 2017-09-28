## getAccounts

`getAccounts` is a very simple tool that simply reports the list of addresses that the local node holds in its keystore. It can be used to self-report on your holdings.

A simple way to effectively use this tool is to feed its output through either getBalance or getTokenBal to report on current holdings. Using the --ether or --dollars command line options give more information. For example, you can do these commands:

List accounts on the local node:

    getAccounts

List balances of the local node's accounts:

    getBalance `getAccounts | xargs`
    
List balances of the local node's accounts in US dollars:

    getBalance `getAccounts | xargs` --dollars

List token balances on the Singular DTV token for the local node's accounts in ether:

    getTokenBal `ethName -a singular` `getAccounts | xargs` --ether

#### Usage

`Usage:`    getAccounts [-a|-v|-h]  
`Purpose:`  Show the list of Ethereum accounts known to the local node.  
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
| -a | --alone | Show only the addresses for use in scripting |
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


