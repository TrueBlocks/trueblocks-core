## isContract

This simple program may be used to query an Ethereum address to determine if it is a `smart contract` or simply a regular `external account`. It may also be used to retrieve the byte-code from an address (if present). Finally, it may be used to compare the byte code found at two addresses to determine if they hold identical code. You may specify multiple addresses on a line.

#### Usage

`Usage:`    isContract [-d|-n|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Returns 'true' or 'false' if the given address(es) holds byte code (optionally displays the code).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | a space-separated list of one or more Ethereum addresses |
| -d | --display | display the byte code at the address(es) |
| -n | --nodiff | return 'true' if (exactly) two Ethereum addresses have identical code |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `Addresses` must start with '0x' and be forty characters long.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured.
- If the queried node does not store historical state, the results are undefined.
- `Special` blocks are detailed under `whenBlock --list`.

`See Also`: This command-line tool implements the [eth_getCode](https://github.com/paritytech/parity/wiki/JSONRPC-eth-module#eth_getcode) RPC interface.

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


