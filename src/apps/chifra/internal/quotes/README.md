## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

```[plaintext]
Purpose:
  Update or display Ethereum price data, this tool has been deprecated.

Usage:
  chifra quotes [flags]

Flags:
  -u, --update          freshen price database
  -p, --period string   increment of display
                        One of [ 5 | 15 | 30 | 60 | 120 | 240 | 1440 | 10080 | hourly | daily | weekly ]
  -a, --pair string     which price pair to freshen or list (see Poloniex)
  -e, --feed string     the feed for the price data
                        One of [ poloniex | maker | tellor ]
  -x, --fmt string      export format, one of [none|json*|txt|csv|api]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - Due to restrictions from Poloniex, this tool retrieves only 30 days of data at a time. You must repeatedly run this command until the data is up-to-date.
```

Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --dollars         export values in US dollars
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
