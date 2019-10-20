## getState

Retrieve the balance (in wei) of an address (or list of addresses) at a given block(s). You may specify multiple addresses and/or multiple blocks, but you must 
specify at least one address.

This simple program may be used to query an Ethereum address to determine if it is a `smart contract` or simply a regular `external account`. It may also be used to retrieve the byte-code from an address (if present). Finally, it may be used to compare the byte code found at two addresses to determine if they hold identical code. You may specify multiple addresses on a line.

#### Usage

`Usage:`    getState [-m|-n|-c|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses (0x...) from which to retrieve balances (required) |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -m | --mode <val> | control which state to export, one of [none&#124;some*&#124;all&#124;balance&#124;nonce&#124;code&#124;storage&#124;deployed&#124;accttype] |
| -n | --no_zero | suppress the display of zero balance accounts |
| -c | --changes | only report a balance when it changes from one block to the next |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- `block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple modes on a single line.

`See Also`: This command-line tool implements the [eth_getCode](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_getcode) RPC interface.

#### Usage

`Usage:`    getState [-m|-n|-c|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses (0x...) from which to retrieve balances (required) |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -m | --mode <val> | control which state to export, one of [none&#124;some*&#124;all&#124;balance&#124;nonce&#124;code&#124;storage&#124;deployed&#124;accttype] |
| -n | --no_zero | suppress the display of zero balance accounts |
| -c | --changes | only report a balance when it changes from one block to the next |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- `block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple modes on a single line.

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

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

**Powered by Qblocks<sup>&trade;</sup>**


