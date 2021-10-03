## chifra slurp

`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from [http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction for a given account. This is actually a shortcoming with EtherScan. It's too complicated to explain here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull any transaction initiated by an EOA for example or to explore mining rewards. Visit the above referenced website for more information.

### Usage

`Usage:`    chifra slurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetch data from EtherScan for any address.

`Where:`

|          | Option                          | Description                                                                                            |
| -------- | ------------------------------- | ------------------------------------------------------------------------------------------------------ |
|          | addrs                           | one or more addresses to slurp from Etherscan (required)                                               |
|          | blocks                          | an optional range of blocks to slurp                                                                   |
| &#8208;t | &#8208;&#8208;types &lt;val&gt; | which types of transactions to request, one or more<br/>of [ext, int, token, nfts, miner, uncles, all] |
| &#8208;p | &#8208;&#8208;appearances       | show only the blocknumer.tx_id appearances of the exported<br/>transactions                            |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                      |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                       |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                               |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

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
