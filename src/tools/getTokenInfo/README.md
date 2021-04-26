## chifra tokens

Given the address of an ERC20 token contract, report token balances for one or more accounts. Alternatively, report token balances for multiple ERC20 contracts for a single account.

In normal operation the **first item** in the `address_list` is considered to be an ERC20 token contract whose balances are queried, whereas the remainder of the list is assumed to be accounts on which to report.

In `--byAcct` mode, **all items** in the `address_list` are assumed to be token contracts, except the last which is the account whose token balances are reported.

You may optionally specify one or more blocks at which to report.

#### Usage

`Usage:`    chifra tokens [-p|-b|-n|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve token balances for one or more address at given block(s).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required) |
|  | blocks | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -p | --parts <val> | one or more parts of the token information to retreive, one or more of [name&#124;symbol&#124;decimals&#124;totalSupply&#124;version&#124;none&#124;all*] |
| -b | --by_acct | consider each address an ERC20 token except the last, whose balance is reported for each token |
| -n | --no_zero | suppress the display of zero balance accounts |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source**: [`tools/getTokenInfo`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTokenInfo)

