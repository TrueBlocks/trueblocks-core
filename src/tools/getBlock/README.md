## getBlock

The `getBlock` tool retrieves Ethereum block data from a running node or, if previously retrieved, the QuickBlocks cache. It optionally retrieves the hashes that denote the transactions in the block or the full transactional data as a default.

The tool may also be used to double check that the results produced from the QuickBlocks cache and the results as retrieved directly from the running node are identical (modulo the fact that QuickBlocks does not store every data field from the node). You may accomplish this `check` with the --check option.

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


