## getBlock

The `getBlock` tool retrieves Ethereum block data from a running node or, if previously retrieved, the QBlocks cache. It optionally retrieves the hashes that denote the transactions in the block or the full transactional data as a default.

The tool may also be used to double check that the results produced from the QBlocks cache and the results as retrieved directly from the running node are identical (modulo the fact that QBlocks does not store every data field from the node). You may accomplish this `check` with the --check option.

#### Usage

`Usage:`    getBlock [-n|-c|-a|-u|-q|-m|-i|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | a space-separated list of one or more blocks to retrieve (required) |
| -n | --hash_only | display only transaction hashes, default is to display full transaction detail |
| -c | --check | compare results between qblocks and Ethereum node, report differences, if any |
| -a | --addrs | display all addresses included in the block |
| -u | --uniq | display only uniq addresses found per block |
| -q | --uniq_tx | display only uniq addresses found per transaction |
| -m | --count_only | display counts of appearances (for --addrs, --uniq, or --uniq_tx only) |
| -i | --filter <addr> | useful only for --addrs or --uniq, only display this address in results |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `block_list` is a space-separated list of values, a start-end range, a `special`, or any combination.
- This tool retrieves information from the local node or rpcProvider if configured (see documentation).
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
    --veryRaw   |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


