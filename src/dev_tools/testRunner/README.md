## chifra testRunner

`grabABI` retrieves an ABI file either from a local cache, from the Ethereum Name Service (ENS), or from [Etherscan](http://etherscan.io). Once retrieved, `grabABI` generates the classDefinition files needed by [makeClass](../makeClass/README.md) to generate a parselib for each [Ethereum address monitor](../../monitors/README.md).

#### Usage

`Usage:`    testRunner [-m|-f|-c|-n|-o|-r|-v|-h]  
`Purpose:`  Run TrueBlocks' test cases with options.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -m | --mode <val> | determine which set of tests to run, one of [cmd*&#124;api&#124;both] |
| -f | --filter <val> | determine how long it takes to run tests, one of [fast*&#124;medi&#124;slow&#124;all] |
| -c | --clean | clean working folder before running tests |
| -n | --no_quit | do not quit testing on first error |
| -o | --no_post | do not complete the post processing step |
| -r | --report | display performance report to screen |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

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

**Powered by TrueBlocks<sup>&trade;</sup>**


