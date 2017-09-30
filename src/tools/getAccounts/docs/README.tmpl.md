## [{NAME}]

*Published: Saturday, Sept. 29, 2017 - 12:03 am*

`[{NAME}]` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [ethName](../ethName/README.md).

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

[{USAGE_TABLE}][{FOOTER}]
