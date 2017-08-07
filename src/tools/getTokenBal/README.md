## getTokenBal

For the given list of token contracts, retrieve the holders' token balance at the given block(s). You may specify multiple contracts, holder addresses, or blocks on the line, but not more than one multiple. `block`, if not specified, defaults to `latest`.

If the token contract(s) you're requesting are not ERC20 token standard contracts, the return values are undefined.

`getTokenBal` retrieves a token's balance from the local running Ethereum node (not QuickBlocks). Use the `--accounting` option of an account monitor to retrieve the balances from QuickBlocks.

#### Usage

`Usage:`    getTokenBal [-d|-e|-v|-h] token holders blocks  
`Purpose:`  Retrieve the token balance from a token contract for an account at a block (or more).
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | token | token contract (starting with '0x') from which to retrieve the balance |
|  | holders | holder address(es) (starting with '0x') for which to retrieve the balance(s) |
|  | blocks | the block at which to retrieve the token balance(s) (defaults to 'latest') |
| -d | --data | render results as tab delimited data |
| -e | --ether | return the token balance in Ether instead of Wei |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### Other Options

Enter `--version` to display the current version of the tool.  
Enter `--nocolors` to turn off colored display.  
Enter `--wei` (default), `--ether`, or `--dollars` to alter the way value is displayed.  

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Place a semi-colon at the start of a line to make it a comment.

#### Powered by QuickBlocks&reg;
