## [{NAME}]

`ethName` provides a very simple, wholy-inadequate method to keep track of 'real life' names and their associated Ethereum addresses. While this tool needs a lot of work, it does come in handy. Eventually, we will transition this tool to use ENS (or an equivalent).

One way to use this tool is to feed its output into another tool's input. For example, the following command would open the [EtherScan](http://etherscan.io) website to the address associated with The DAO:

    ethscan.py `ethName -a DAO`

[{USAGE_TABLE}]#### Notes

With a single search term, the tool searches both `name` and `address`. With two search terms, the first must match the `address` field, and the second must match the `name` field. When there are two search terms, both must match.

If one mixes options, the `--edit` option always predominates (i.e. the program opens the database and then quits).

The `--list` option predominates otherwise. If present, the tool displays a list of stored names and addresses and then quits.

The `--count` option works with any other option and will simply display the number of matches or '0 matches' if 
none.

The `--matchCase` option requires case sensitive matching. It works with all other options.

The `--addrOnly` option modifies the display output and therefore works with any other options.

[{FOOTER}]
