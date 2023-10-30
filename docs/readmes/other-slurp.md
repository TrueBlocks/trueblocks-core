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
                        One or more of [ ext | int | token | nfts | 1155 | miner | uncles | withdrawals | all ]
  -p, --appearances     show only the blocknumber.tx_id appearances of the exported transactions
  -P, --per_page uint   the number of records to request on each page (default 5000)
  -s, --sleep float     seconds to sleep between requests (default 0.25)
  -H, --ether           specify value in ether
  -w, --raw             report JSON data from the source with minimal processing
  -o, --cache           force the results of the query into the cache
  -D, --decache         removes related items from the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose output
  -h, --help            display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Portions of this software are Powered by Etherscan.io APIs.
  - The withdrawals option is only available on certain chains. It is ignored otherwise.
```

Data models produced by this tool:

- [slurp](/data-model/other/#slurp)

Links:

- [api docs](/api/#operation/other-slurp)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/slurp)

