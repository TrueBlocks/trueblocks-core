## ENS

This folder contains a QuickBlocks monitor. QuickBlocks monitors pull transactions from the Ethereum blockchain for a given (or a series of) Ethereum addresses.

Below we present the command line interface to this tool, although the tool itself is not available under open source. While the tool is in active development, QuickBlocks monitors already produce very useful results. For example, we use QuickBlocks monitors to account for and analyze all transactions on a given smart contract. We present [this example](http://dao.quickblocks.io).

Please contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.

#### Usage

`Usage:`    ENS [-f|-s|-c|-k|-o|-p|-g|-t|-b|-a|-l|-d|-s|-r|-v|-h]  
`Purpose:`  Index transactions for a given Ethereum address (or series of addresses).
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -f | --freshen | mode: freshen only -- default (do not display transactions from the cache) |
| -s | --showCache | mode: show transactions from the cache, and then freshen |
| -c | --cacheOnly | mode: display transactions from the cache only (do not freshen) |
| -k | --kBlock val | start processing at block :k |
| -o | --offset val | offset to kBlock |
| -p | --parse | display parsed input data |
| -g | --logs | display smart contract lo(g)s or events |
| -t | --trace | display smart contract internal traces |
| -b | --bloom | display bloom filter matching |
| -a | --accounting | display credits and debits per account and reconcile at each block |
| -l | --list | display list of monitored accounts |
| -d | --debug | enter debug mode (pause after each transaction) |
| -s | --single | if debugging is enable, single step through transactions |
| -r | --rebuild | clear cache and reprocess all transcations (may take a long time) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

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


