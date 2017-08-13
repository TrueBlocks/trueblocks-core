## [{NAME}]

For the given list of token contracts, retrieve the holders' token balance at the given block(s). You may specify multiple contracts, holder addresses, or blocks on the line, but not more than one multiple. `block`, if not specified, defaults to `latest`.

If the token contract(s) you're requesting are not ERC20 token standard contracts, the return values are undefined.

`getTokenBal` retrieves a token's balance from the local running Ethereum node (not QuickBlocks). Use the `--accounting` option of an account monitor to retrieve the balances from QuickBlocks.
[{COMMAND_HELP}]
