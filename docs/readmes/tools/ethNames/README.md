## chifra names

`chifra names` lists the addresses found in your local node's keystore. It can be used to report your ether holdings, for example. It also lists known, named accounts from [chifra names](../ethName/README.md).

One way to use this tool is to feed its output through the [chifra state](../getState/README.md) or [chifra tokens](../getTokens/README.md). This will give you the balances of your ether holdings or token holings. For example, you can do these commands:

List accounts held in the local node's keystore:

    chifra names

List balances of those accounts (note 'xargs' puts the results on a single line):

    chifra state --mode balance `chifra names -a | xargs`
    
List balances of the local node's accounts in US dollars:

    chifra state --mode balance `chifra names -a | xargs` --dollars

Using chifra names to find Singular's address, list token balances held by your accounts:

    chifra tokens `chifra names -a singular` `chifra names | xargs`

Using chifra names to find Singular's address, list tokens held by other token accounts:

    chifra tokens `chifra names -a singular` `chifra names -n | xargs`

### usage

`Usage:`    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses and/or names of well known accounts.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | terms | a space separated list of one or more search terms (required) |
| -e | --expand | expand search to include all fields (default searches name, address, and symbol only) |
| -m | --match_case | do case-sensitive search |
| -l | --all | include all accounts in the search |
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
- Name file: `$CONFIG/names/names.tab`

**Source code**: [`tools/ethNames`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethNames)

