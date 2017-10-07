## getBlock

`ethName` provides a very simple, poorly-done method by which to keep track of 'real life' names and their associated Ethereum addresses. While the tools needs a lot of work, it has come in handy as we've been writing **quickBlocks**. Eventually a feature such as this will be replaced with ENS (or an equivalent), but in the mean time, `ethname` works well to help you remember who is associated with which addresses.

A very useful way to use of this tools is to feed its output into another command-line tool's input. For example, you could make a command such as:

    ethname -a 'FredJones' | ethscan

which would open the [etherscan](http://etherscan.io) website with the address associated with Fred's address(es).

#### Usage

`Usage:`    getBlock [-r|-p|-t|-c|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks to retrieve |
| -r | --raw | pull the block data from the running Ethereum node (no cache) |
| -p | --parity | mimic parity output (i.e. quoted hexidecimal for numbers, etc.) |
| -t | --terse | display only transaction hashes, default is to display full transaction details |
| -c | --check | compare results between qblocks and Ethereum node, report differences, if any |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination
- this tool retrieves information from the local node or the ${FALLBACK} node, if configured (see documentation)
- `special` blocks are detailed under `whenBlock --list`

#### Notes

With one search term, search in both `name` and `address` fields. With two terms, the first must match the `address` field, while the second must match the `name` field. When there are two search terms, both must match.

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


