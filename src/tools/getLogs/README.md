## getLogs

`getLogs` returns a transaction's logs (also called events). You may specify the logs you want using either a transaction hash (trans\_hash), a block hash plus transaction index (block\_hash.trans\_id), or a block number plus transaction index (block\_num.trans\_id).  

#### Usage

`Usage:`    getLogs [-r|-v|-h] &lt;transID&gt; [transID...]  
`Purpose:`  Retrieve a transaction's logs from the local cache or a running node.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | trans_list | a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID) |
| -r | --raw | retrieve raw transaction directly from the running node |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `trans_list` is one or more space-separated identifiers which may be either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested exists.
- This tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation).
- If the queried node does not store historical state, the results may be undefined.

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


