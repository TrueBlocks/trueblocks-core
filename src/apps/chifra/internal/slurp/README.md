## chifra slurp

<!-- markdownlint-disable MD041 -->
`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
[http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
for a given account. This is actually a shortcoming with Etherscan. It's too complicated to explain
here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull
any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
referenced website for more information.

```[plaintext]
Purpose:
  Fetch data from Etherscan for any address.

Usage:
  chifra slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp

Flags:
  -t, --types strings   which types of transactions to request
                        One or more of [ ext | int | token | nfts | 1155 | miner | uncles | all ]
  -p, --appearances     show only the blocknumber.tx_id appearances of the exported transactions
  -P, --per_page uint   the number of records to request on each page (default 5000)
  -s, --sleep float     seconds to sleep between requests (default 0.25)
  -w, --raw             report JSON data from the source with minimal processing
  -o, --cache           force the results of the query into the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Portions of this software are Powered by Etherscan.io APIs.
```

Data models produced by this tool:

- [transaction](/data-model/chaindata/#transaction)

<!-- markdownlint-disable MD041 -->
### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
  ```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

