## getAccountInfo

This simple program may be used to query an Ethereum address to determine if it is a `smart contract` or simply a regular `external account`. It may also be used to retrieve the byte-code from an address (if present). Finally, it may be used to compare the byte code found at two addresses to determine if they hold identical code. You may specify multiple addresses on a line.

#### Usage

`Usage:`    getAccountInfo [-d|-b|-n|-w|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) holds byte code (optionally displays the code).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -d | --data | display results as data (addr <tab> is_contract) |
| -b | --bytes | display the byte code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
| -w | --whenDep | for smart contracts only, return the first block when the address had code |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.
- `block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.

`See Also`: This command-line tool implements the [eth_getCode](https://github.com/ethereum/wiki/wiki/JSON-RPC#eth_getcode) RPC interface.

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
    --veryRaw   |   report JSON data from node with zero processing
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


