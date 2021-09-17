## chifra slurp

`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from [http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction for a given account. This is actually a shortcoming with EtherScan. It's too complicated to explain here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull any transaction initiated by an EOA for example or to explore mining rewards. Visit the above referenced website for more information.

### Usage

`Usage:`    chifra slurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetch data from EtherScan for any address.

`Where:`  

|     | Option              | Description                                                                                                                |
| --- | ------------------- | -------------------------------------------------------------------------------------------------------------------------- |
|     | addrs               | one or more addresses to slurp from Etherscan (required)                                                                   |
|     | blocks              | an optional range of blocks to slurp                                                                                       |
| -t  | --types &lt;val&gt; | one or more types of transactions to request, one or more of *[ ext\* \| int \| token \| nfts \| miner \| uncles \| all ]* |
| -p  | --appearances       | show only the blocknumer.tx_id appearances of the exported transactions                                                    |
| -x  | --fmt &lt;val&gt;   | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                                                            |
| -v  | --verbose           | set verbose level (optional level defaults to 1)                                                                           |
| -h  | --help              | display this help screen                                                                                                   |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

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

**Source code**: [`tools/ethslurp`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethslurp)

