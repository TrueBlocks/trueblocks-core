## ethName

`ethName` provides a simple way to keep track of 'real life' names and their associated Ethereum addresses. While this tool will eventually use the [Ethereum Name Service](http://ens.domains) directly to find and manage names, it currently uses a locally-editable text database to build a name-address map.

You may feed the output of this tool into another tool's input. For example, the following command would open the [EtherScan](http://etherscan.io) website to the address associated with The DAO:

    ethscan.py `ethName -a DAO`

#### Usage

`Usage:`    ethName [-a|-d|-e|-f|-l|-m|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query Ethereum addresses and/or names making it easy to remember accounts.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | terms | a space separated list of one or more search terms |
| -a | --addr | export only the associated address (may be used in scripting) |
| -d | --data | export results as tab separated data |
| -e | --edit | open the name database for editing |
| -f | --fmt val | export format (one of [json&#124;txt&#124;csv]) |
| -l | --list | list all names in the database |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- With a single search term, the tool searches both `name` and `address`.
- With two search terms, the first term must match the `address` field, and the second term must match the `name` field.
- When there are two search terms, both must match.
- If one mixes options, the `--edit` option predominates (i.e. the program opens the database and then quits).
- The `--list` option predominates otherwise. If present, the tool displays a list of stored names and addresses and then quits.
- The `--count` option works with any other option and will simply display the number of matches.
- The `--matchCase` option requires case sensitive matching. It works with all other options.
- Name file: `~/.quickBlocks/names/names.txt` (167896)

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


