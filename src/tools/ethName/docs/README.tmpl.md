## [{NAME}]

`ethName` provides a simple way to keep track of 'real life' names and their associated Ethereum addresses. While this tool will eventually use the ([Ethereum Name Service](http://ens.domains)) to find and manage names, it currently uses a locally-editable database to start a name-address map.

You way to use this tool is to feed its output into another tool's input. For example, the following command would open the [EtherScan](http://etherscan.io) website to the address associated with The DAO:

    ethscan.py `ethName -a DAO`

[{USAGE_TABLE}][{FOOTER}]
