## chifra status

The `chifra status` program allows you to manage the various TrueBlock caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called collections.

### Usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on the status of the TrueBlocks system.

`Where:`

|          | Option                          | Description                                                                                                                |
| -------- | ------------------------------- | -------------------------------------------------------------------------------------------------------------------------- |
|          | modes                           | the type of status info to retrieve, one or more of<br/>[index, monitors, collections, names, abis, caches,<br/>some, all] |
| &#8208;d | &#8208;&#8208;details           | include details about items found in monitors, slurps,<br/>abis, or price caches                                           |
| &#8208;t | &#8208;&#8208;types &lt;val&gt; | for caches mode only, which type(s) of cache to report,<br/>one or more of [blocks, txs, traces, slurps, prices,<br/>all]  |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                                          |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                                           |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                                   |

#### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --wei       | export values in wei (the default)                            |
| --ether     | export values in ether                                        |
| --dollars   | export values in US dollars                                   |
| --raw       | pass raw RPC data directly from the node with no processing   |
| --to_file   | write the results to a temporary file and return the filename |
| --output fn | write the results to file 'fn' and return the filename        |
| --file fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file fn` option, you may place a series of valid command lines in a file using any of the above flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes that line a comment.*</small>
