## [{NAME}]

Given a list of Ethereum addresses, report token holdings for a single `account` from one or more `token contracts`. Alternatively, report token balances for one or more `accounts` from a single `token contract`.

In normal operation the *last item* in the `address_list` is considered the account for which to report token balances from each of the preceeding token contracts in `address_list`.

In `--flip` mode, the *first item* in `address_list` is considered the token contract from which to report token balances for the remaining accounts in the list. At least two addresses are required in `address_list`.

You may optionally specify one or more blocks at which to report token balances. If empty, `block_list` defaults to `latest`.

[{USAGE_TABLE}]`Notes:`

- If an address does not own tokens at an address, the tool reports a zero balance.
- If the token contract(s) from which you request balances are not ERC20 token contracts, the return values are undefined.
- This tool retrieves information from a locally running Ethereum node or the $(FALLBACK) node, if enabled.[{FOOTER}]
