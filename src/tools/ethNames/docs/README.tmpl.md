## [{NAME}]

`[{NAME}]` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [chifra names](../ethName/README.md).

One way to use this tool is to feed its output through the [chifra state](../getState/README.md) or [chifra tokens](../getTokenInfo/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    [{NAME}]

List balances of those accounts (note 'xargs' puts the results on a single line):

    chifra state --mode balance `[{NAME}] -a | xargs`
    
List balances of the local node's accounts in US dollars:

    chifra state --mode balance `[{NAME}] -a | xargs` --dollars

Using [{NAME}] to find Singular's address, list token balances held by your accounts:

    chifra tokens `[{NAME}] -a singular` `[{NAME}] | xargs`

Using [{NAME}] to find Singular's address, list tokens held by other token accounts:

    chifra tokens `[{NAME}] -a singular` `[{NAME}] -n | xargs`

[{USAGE_TABLE}][{FOOTER}]
