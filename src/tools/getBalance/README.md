## getBalance

Retrieve the balance (in wei) of an address (or list of addresses) at a given block(s). You may specify multiple addresses and/or multiple blocks, but you must 
specify at least one address.

#### Usage

`Usage:`    getBalance [-d|-l fn|-n|-t|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve the balance (in wei) for one or more addresses at the given block(s).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | address_list | one or more addresses (0x...) from which to retrieve balances |
|  | block_list | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -d | --data | render results as tab delimited data |
| -l | --list fn | an alternative way to specify an address_list; place one address per line in the file 'fn' |
| -n | --noZero | suppress the display of zero balance accounts |
| -t | --total | if more than one balance is requested, display a total as well. |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty characters long.
- `block_list` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `whenBlock --list`.

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


