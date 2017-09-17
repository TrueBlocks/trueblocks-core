## getTokenBal

Given a list of Ethereum addresses, report token holdings for a single `account` from one or more `token contracts`. Alternatively, report token balances for one or more `accounts` from a single `token contract`.

In normal operation the *last item* in the `address_list` is considered the account for which to report token balances from each of the preceeding token contracts in `address_list`.

In `--flip` mode, the *first item* in `address_list` is considered the token contract from which to report token balances for the remaining accounts in the list. At least two addresses are required in `address_list`.

You may optionally specify one or more blocks at which to report token balances. If empty, `block_list` defaults to `latest`.

#### Usage

`Usage:`    getTokenBal [-b|-d|-v|-h] address_list block_list  
`Purpose:`  Retrieve token balances from a series of ERC20 token contracts for an account or visa versa.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | address_list | two or more address (0x...), the first is an ERC20 token, balances for the remaining accounts are reported |
|  | block_list | a list of one or more blocks at which to report balances, if empty defaults to 'latest' |
| -b | --byAcct | if enabled, all but the last address is considered an ERC20 token, balances for each for the last address are reported. |
| -d | --data | render results as tab delimited data (for example, to build a cap table) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- If an address does not own tokens at an address, the tool reports a zero balance.
- If the token contract(s) from which you request balances are not ERC20 token contracts, the return values are undefined.
- This tool retrieves information from a locally running Ethereum node or the $(FALLBACK) node, if enabled.#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


