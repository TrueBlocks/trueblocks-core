## [{NAME}]

`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an equivalent), but in the mean time, `ethname` works well to help you remember who is associated with which addresses.

A very useful way to use of this tools is to feed its output into another command-line tool's input. For example, you could make a command such as:

    ethname -a 'FredJones' | ethscan

which would open the [etherscan](http://etherscan.io) website with the address associated with Fred's address(es).

[{COMMAND_HELP}]

#### Notes

With one search term, search in both `name` and `address` fields. With two terms, the first must match the `address` field, while the second must match the `name` field. When there are two search terms, both must match.

If one mixes options, the `edit` option always predominates. If present, the program opens the database and then quits. The `--list` option predominates otherwise. If 
present the tool will display a list of all items. The `--count` option works with any other option and will simply display the number of matches or '0 matches' if 
none. The `--matchCase` option requires case sensitive matching. It works with all other options. The `--addrOnly` option modifies the display output and therefore 
works with any other options.
