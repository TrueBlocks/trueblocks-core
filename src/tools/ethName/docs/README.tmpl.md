## [{NAME}]

`ethName` provides a simple way to keep track of 'real life' names and their associated Ethereum addresses. While this tool will eventually use the [Ethereum Name Service](http://ens.domains) directly to find and manage names, it currently uses a locally-editable text database to build a name-address map.

You may feed the output of this tool into another tool's input. For example, the following command would open the [EtherScan](http://etherscan.io) website to the address associated with The DAO:

    ethscan.py `ethName -a DAO`

[{USAGE_TABLE}][{FOOTER}]
