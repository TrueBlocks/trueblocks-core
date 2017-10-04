## getTokenBal

*Published: Saturday, Sept. 29, 2017 - 12:03 am*

Given the address of an ERC20 token contract, report token balances for one or more accounts. Alternatively, report token balances for multiple ERC20 contracts for a single account.

In normal operation the **first item** in the `address_list` is considered to be an ERC20 token contract whose balances are queried, whereas the remainder of the list is assumed to be accounts on which to report.

In `--byAcct` mode, **all items** in the `address_list` are assumed to be token contracts, except the last which is the account whose token balances are reported.

You may optionally specify one or more blocks at which to report.

#### Usage

`Usage:`    getTokenBal [-b|-l fn|-n|-d|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the token balance(s) for one or more addresses at the given (or latest) block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -b | --byAcct | consider each address an ERC20 token except the last, whose balance is reported for each token |
| -l | --list fn | an alternative way to specify an address_list, place one address per line in the file 'fn' |
| -n | --noZero | suppress the display of zero balance accounts |
| -d | --data | render results as tab delimited data (for example, to build a cap table) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty characters long
- `block_list` may be space-separated list of values, a start-end range, a `special`, or any combination
- this tool retrieves information from the local node or the ${FALLBACK} node, if configured (see the documentation)
- if the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined
- if the queried node does not store historical state, the results are undefined
- `special` blocks are detailed under `whenBlock --list`

#### Other Options

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


