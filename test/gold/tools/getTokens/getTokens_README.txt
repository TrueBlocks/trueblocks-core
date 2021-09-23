getTokens argc: 2 [1:--readme] 
getTokens --readme 
### Usage

`Usage:`    chifra tokens [-p|-b|-n|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve token balance(s) for one or more addresses at given block(s).

`Where:`

{{<td>}}
|          | Option                          | Description                                                                                                                              |
| -------- | ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
|          | addrs                           | two or more addresses (0x...), the first is an ERC20<br/>token, balances for the rest are reported (required)                            |
|          | blocks                          | an optional list of one or more blocks at which to<br/>report balances, defaults to 'latest'                                             |
| &#8208;p | &#8208;&#8208;parts &lt;val&gt; | one or more parts of the token information to retreive,<br/>one or more of [name, symbol, decimals, totalSupply,<br/>version, none, all] |
| &#8208;b | &#8208;&#8208;by_acct           | consider each address an ERC20 token except the last,<br/>whose balance is reported for each token                                       |
| &#8208;n | &#8208;&#8208;no_zero           | suppress the display of zero balance accounts                                                                                            |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                                                        |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                                                         |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                                                 |
{{</td>}}

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.

