## [{NAME}]

`[{NAME}]` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [ethName](../ethName/README.md).

One way to use this tool is to feed its output through the [getState](../getState/README.md) or [getTokenInfo](../getTokenInfo/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    ethNames

List balances of those accounts (note 'xargs' puts the results on a single line):

    getState --mode balance `ethNames -a | xargs`
    
List balances of the local node's accounts in US dollars:

    getState --mode balance `ethNames -a | xargs` --dollars

Using ethName to find Singular's address, list token balances held by your accounts:

    getTokenInfo `ethName -a singular` `ethNames | xargs`

Using ethName to find Singular's address, list tokens held by other token accounts:

    getTokenInfo `ethName -a singular` `ethNames -n | xargs`

[{USAGE_TABLE}][{FOOTER}]
