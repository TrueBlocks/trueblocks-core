## chifra status

The `chifra status` program allows you to manage the various TrueBlock caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called collections.

### Usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on the status of the TrueBlocks system.

`Where:`  

|     | Option              | Description                                                                                                                        |
| --- | ------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
|     | modes               | the type of status info to retrieve, one or more of<br/>[index, monitors, collections, names, abis, caches,<br/>some all]          |
| -d  | --details           | include details about items found in monitors, slurps,<br/>abis, or price caches                                                   |
| -t  | --types &lt;val&gt; | for caches mode only, which type(s) of cache to report,<br/>one or more of [blocks, transactions, traces, slurps,<br/>prices, all] |
| -x  | --fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                                                  |
| -v  | --verbose           | set verbose level (optional level defaults to 1)                                                                                   |
| -h  | --help              | display this help screen                                                                                                           |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`apps/cacheStatus`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/cacheStatus)

