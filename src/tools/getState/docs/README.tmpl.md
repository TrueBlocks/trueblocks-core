## [{NAME}]

Retrieve the balance (in wei) of an address (or list of addresses) at a given block(s). You may specify multiple addresses and/or multiple blocks, but you must specify at least one address.

This simple program may be used to query an Ethereum address to determine if it is a `smart contract` or simply a regular `external account`. It may also be used to retrieve the byte-code from an address (if present). Finally, it may be used to compare the byte code found at two addresses to determine if they hold identical code. You may specify multiple addresses on a line.

[{USAGE_TABLE}]`See Also`: This command-line tool implements the [eth_getCode](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_getcode) RPC interface.

[{USAGE_TABLE}][{FOOTER}]
