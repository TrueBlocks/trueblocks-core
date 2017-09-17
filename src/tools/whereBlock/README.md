## whereBlock

`whereBlock` reports on the location of a **quickBlocks** block (or blocks). It will return either `cache`, `node`, `remote`, or `none` depending on where it finds the block.

If it finds the block in its own cache, it reports the path to the file in the `cache`. If it finds the block on a locally-running node, it reports `local` followed by the name and version of the locally running node. If the block is not found (that is, the block is not in the cache and there is no locally-running node), it reports 'remote' if the value of the environment variable `$(FALLBACK)` is set and a remote node is found. Otherwise, `whereBlock` reports `none`.

When `whereBlock` reports `remote`, it is reporting on the running node found at the value of the `$(FALLBACK)` environment variable, which currently accepts only a value of `infura`.

This tool is intended mostly as an aid in developing and debugging **quickBlocks** and related tools.

#### Usage

`Usage:`    whereBlock [-a|-v|-h] block[s]  
`Purpose:`  Reports if a block was found in the cache or at a local or remote node.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | block[s] | a space-separated list of one or more blocks to search for |
| -a | --alone | display the path(s) of the blocks if found in the cache |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

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


