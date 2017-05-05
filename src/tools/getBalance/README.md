## getBalance

Retrieve the balance of an address (or more than one) at the given block(s). You may specify multiple addresses or multiple blocks on the line, but not both. `block`, if not specified, defaults to `latest`.

`getBalance` retrieves the balance from the local Ethereum node (not QuickBlocks). Use the `--accounting` option of an account monitor to retrieve the balance from QuickBlocks (both balances should always agree, obviously).

#### Help Screen

`Usage:`    getBalance [-d|-e|-v|-h] addr(s) block(s)  
`Purpose:`  Retrieve the balance for an account at a block.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr(s) | Ethereum address (starting with '0x') from which to retrieve the balance |
|  | block(s) | the block at which to retrieve the balance (defaults to 'latest') |
| -d | --data | render results as tab delimited data |
| -e | --ether | return the balance in Ether instead of Wei |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Please a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
