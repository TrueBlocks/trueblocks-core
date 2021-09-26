## chifra tokens

Given the address of an ERC20 token contract, this tool reports token balances for one or more additional addresses. Alternatively, the tool can report the token balances for multiple ERC20 tokens for a single addresses.

In normal operation the **first item** in the `address_list` is assumed to be an ERC20 token contract whose balances are being queried, whereas the remainder of the list is assumed to be addresses on which to report.

In `--byAcct` mode, **all addresses** in the `address_list` are assumed to be ERC20 token contracts, except the final one which is the account whose token balances are reported.

You may optionally specify one or more blocks at which to report. If no block is specified, the latest block is assumed. You may also optionally specify which parts of the token data to extract.

### Usage

`Usage:`    chifra tokens [-p|-b|-n|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve token balance(s) for one or more addresses at given block(s).

`Where:`

|          | Option                          | Description                                                                                                                        |
| -------- | ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
|          | addrs                           | two or more addresses (0x...), the first is an ERC20<br/>token, balances for the rest are reported (required)                      |
|          | blocks                          | an optional list of one or more blocks at which to<br/>report balances, defaults to 'latest'                                       |
| &#8208;p | &#8208;&#8208;parts &lt;val&gt; | which parts of the token information to retreive, one<br/>or more of [name, symbol, decimals, totalSupply, version,<br/>none, all] |
| &#8208;b | &#8208;&#8208;by_acct           | consider each address an ERC20 token except the last,<br/>whose balance is reported for each token                                 |
| &#8208;n | &#8208;&#8208;no_zero           | suppress the display of zero balance accounts                                                                                      |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                                                  |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                                                   |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                                           |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --nocolor   | turn off colored display                                      |
| --wei       | specify value in wei (the default)                            |
| --ether     | specify value in ether                                        |
| --dollars   | specify value in US dollars                                   |
| --raw       | report JSON data from the node with minimal processing        |
| --to_file   | write the results to a temporary file and return the filename |
| --output:fn | write the results to file 'fn' and return the filename        |
| --file:fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/getTokens`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTokens)

