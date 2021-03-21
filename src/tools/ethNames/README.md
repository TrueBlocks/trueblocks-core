## ethNames

`ethNames` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [ethName](../ethName/README.md).

One way to use this tool is to feed its output through the [getState](../getState/README.md) or [getTokenInfo](../getTokenInfo/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    ethNames

List balances of those accounts (note 'xargs' puts the results on a single line):

    getState --mode balance `ethNames -a | xargs`
    
List balances of the local node's accounts in US dollars:

    getState --mode balance `ethNames -a | xargs` --dollars

Using ethName to find Singular's address, list token balances held by your accounts:

    getTokenInfo `ethName -a singular` `ethNames | xargs`

Using ethName to find Singular's address, list tokens held by other token accounts:

    getTokenInfo `ethName -a singular` `ethNames -n | xargs`

#### Usage

`Usage:`    ethNames [-e|-m|-l|-o|-c|-p|-n|-a|-s|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses and/or names of well known accounts.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | terms | a space separated list of one or more search terms (required) |
| -e | --expand | expand search to include all fields (default searches name, address, and symbol only) |
| -m | --match_case | do case-sensitive search |
| -l | --all | include all accounts in the search |
| -o | --owned | include personal accounts in the search |
| -c | --custom | include your custom named accounts |
| -p | --prefund | include prefund accounts |
| -n | --named | include well know token and airdrop addresses in the search |
| -a | --addr | display only addresses in the results (useful for scripting) |
| -s | --entities | display entity data |
| -g | --tags | export the list of tags and subtags only |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- With a single search term, the tool searches both `name` and `address`.
- With two search terms, the first term must match the `address` field, and the second term must match the `name` field.
- When there are two search terms, both must match.
- The `--match_case` option requires case sensitive matching. It works with all other options.
- To customize the list of names add a `custom` section to the config file (see documentation).
- Name file: `~/.quickBlocks/names/names.tab` (1153506)

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

**Powered by TrueBlocks<sup>&trade;</sup>**


