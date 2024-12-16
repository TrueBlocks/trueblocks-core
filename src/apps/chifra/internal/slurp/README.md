## chifra slurp

`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
[http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
for a given account. This is actually a shortcoming with API providers. It's too complicated to explain
here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull
any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
referenced website for more information.

Currently supported API providers:

- [TrueBlocks Key](https://key.trueblocks.io)
- Etherscan
- Covalent
- Alchemy

```[plaintext]
Purpose:
  Fetch data from Etherscan and other APIs for any address.

Usage:
  chifra slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp

Flags:
  -r, --parts strings    which types of transactions to request
                         One or more of [ ext | int | token | nfts | 1155 | miner | uncles | withdrawals | some | all ]
  -p, --appearances      show only the blocknumber.tx_id appearances of the exported transactions
  -a, --articulate       articulate the retrieved data if ABIs can be found
  -S, --source string    the source of the slurped data
                         One of [ etherscan | key | covalent | alchemy ] (default "etherscan")
  -U, --count            for --appearances mode only, display only the count of records
  -g, --page uint        the page to retrieve (page number)
      --page_id string   the page to retrieve (page ID)
  -P, --per_page uint    the number of records to request on each page (default 1000)
  -s, --sleep float      seconds to sleep between requests (default 0.25)
  -H, --ether            specify value in ether
  -o, --cache            force the results of the query into the cache
  -D, --decache          removes related items from the cache
  -x, --fmt string       export format, one of [none|json*|txt|csv]
  -v, --verbose          enable verbose output
  -h, --help             display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Portions of this software are Powered by Etherscan.io, Covalent, Alchemy, TrueBlocks Key APIs.
  - See slurp/README on how to configure keys for API providers.
  - The withdrawals option is only available on certain chains. It is ignored otherwise.
  - If the value of --source is key, --parts is ignored.
  - The --types option is deprecated, use --parts instead.
```

Data models produced by this tool:

- [appearance](/data-model/accounts/#appearance)
- [function](/data-model/other/#function)
- [monitor](/data-model/accounts/#monitor)
- [parameter](/data-model/other/#parameter)
- [slurp](/data-model/other/#slurp)

### Adding provider API key

Call `chifra config edit` to edit the configuration file.

For TrueBlocks Key, add `keyEndpoint = "your-key-endpoint-url"` to `chains.mainnet` section.

For all other providers add an entry to `keys` section like this:

```toml
[keys]
  [keys.etherscan]
    apiKey = "etherscan-apikey"
  [keys.covalent]
    apiKey = "covalent-apikey"
  [keys.alchemy]
    apiKey = "alchemy-apikey"
```

### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

_Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker._
