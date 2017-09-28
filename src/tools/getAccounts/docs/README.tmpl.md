## [{NAME}]

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

[{USAGE_TABLE}][{FOOTER}]
