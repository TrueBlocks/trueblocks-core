getTokenBal argc: 2 [1:-th] 
getTokenBal -th 
#### Usage

`Usage:`    getTokenBal [-b|-d|-l fn|-n|-t|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the token balance(s) for one or more addresses at the given (or latest) block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -b | --byAcct | consider each address an ERC20 token except the last, whose balance is reported for each token |
| -d | --data | render results as tab delimited data (for example, to build a cap table) |
| -l | --list fn | an alternative way to specify an address_list, place one address per line in the file 'fn' |
| -n | --noZero | suppress the display of zero balance accounts |
| -t | --total | if more than one balance is requested, display a total as well. |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty characters long.
- `block_list` may be space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.

