## ethName

`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an equivalent), but in the mean time, `ethname` works well to help you remember who is associated with which addresses.

A very useful way to use of this tools is to feed its output into another command-line tool's input. For example to following command would open the [etherscan](http://etherscan.io) website to the transactions associated with Fred's address:

    ethname -a 'FredJones' | ethscan

#### Usage

`Usage:`    ethName [-a|-c|-o|-l|-m|-s|-v|-h] term [name]  
`Purpose:`  Find a name given an Ethereum address, or find an address given a name.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | term [name] | search terms |
| -a | --addrOnly | export only the associated address to be used as input to further commands |
| -c | --count | print only the count of the number of matches |
| -o | --open | open the name database for editing |
| -l | --list | list all names in the database |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -s | --source | search 'source' field as well name and address (the default) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Notes

With one search term, search in both `name` and `address` fields. With two terms, the first must match the `address` field, while the second must match the `name` field. When there are two search terms, both must match.

If one mixes options, the `edit` option always predominates. If present, the program opens the database and then quits. The `--list` option predominates otherwise. If 
present the tool will display a list of all items. The `--count` option works with any other option and will simply display the number of matches or '0 matches' if 
none. The `--matchCase` option requires case sensitive matching. It works with all other options. The `--addrOnly` option modifies the display output and therefore 
works with any other options.

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


