## getBlock

The `getBlock` tool retrieves Ethereum block data from a running node or, if previously retrieved, the QBlocks cache. It optionally retrieves the hashes that denote the transactions in the block or the full transactional data as a default.

#### Usage

`Usage:`    getBlock [-e|-a|-u|-n|-c|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | blocks | a space-separated list of one or more blocks to retrieve (required) |
| -e | --hashes_only | display only transaction hashes, default is to display full transaction detail |
| -a | --addrs | display all addresses included in the block |
| -u | --uniq | display only uniq addresses found per block |
| -n | --uniq_tx | display only uniq addresses found per transaction |
| -c | --count_only | display counts of appearances (for --addrs, --uniq, or --uniq_tx only) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `special` blocks are detailed under `whenBlock --list`.

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
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


