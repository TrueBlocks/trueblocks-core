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
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |         Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.
*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. Use semi-colon make comments.*

#### Powered by QuickBlocks&reg;

