## [{NAME}]

Given a list of Ethereum addresses, report token holdings for a single `account` from one or more `token contracts`. Alternatively, report token balances for one or more `accounts` from a single `token contract`.

In normal operation the *first item* in the `address_list` is considered to be a token smart contract whose token balances are queried, whereas the remainder of addresses in the list are accounts on which to report.

In `--byAcct` mode, the *last item* in `address_list` is considered a regular account whereas the preceeding items are all considered to be ERC20 token contracts from which to report token balances.

You may optionally specify one or more blocks at which to report token balances. If empty, `block_list` defaults to `latest`.

[{USAGE_TABLE}]`Notes:`

- If an address does not own tokens at an address, the tool reports a zero balance.
- If the token contract(s) from which you request balances are not ERC20 token contracts, the return values are undefined.
- This tool retrieves information from a locally running Ethereum node or the $(FALLBACK) node, if enabled.

[{FOOTER}]
