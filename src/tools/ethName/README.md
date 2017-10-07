## ethName

*Published: Saturday, Sept. 29, 2017 - 12:03 am*

`ethName` provides a simple way to keep track of 'real life' names and their associated Ethereum addresses. While this tool will eventually use the ([Ethereum Name Service](http://ens.domains)) to find and manage names, it currently uses a locally-editable database to start a name-address map.

You way to use this tool is to feed its output into another tool's input. For example, the following command would open the [EtherScan](http://etherscan.io) website to the address associated with The DAO:

    ethscan.py `ethName -a DAO`

#### Usage

`Usage:`    ethName [-a|-c|-o|-l|-m|-s|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query Ethereum addresses and/or names making it easy to remember accounts.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | terms | a space separated list of one or more search terms |
| -a | --alone | export only the associated address (may be used in scripting) |
| -c | --count | print only the count of the number of matches |
| -o | --open | open the name database for editing |
| -l | --list | list all names in the database |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -s | --source | search 'source' field as well name and address (the default) |
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
- The `--addrOnly` option modifies the display output and therefore works with any other options.

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


