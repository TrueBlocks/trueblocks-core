## chifra names

`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum addresses. One may ask why this is necessary given that ENS exists. The answer is a single word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of other tools, especially `chifra export` where naming addresses becomes the single best way to turn unintellagable blockchain data into understandable information.

The various options allow you to search and filter the results. The `entities` and `tags` options are used primarily by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

### Usage

`Usage:`    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses or names of well known accounts.

`Where:`  

| | Option | Description |
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
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The tool will accept up to three terms, each of which must match against any field in the database.
- The `--match_case` option enables case sensitive matching.
- Name file: `$CONFIG/names/names.tab`

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/ethNames`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethNames)

