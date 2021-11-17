## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

```[plaintext]
Purpose:
  Freshen or display Ethereum price data. This tool has been deprecated.

Usage:
  chifra quotes [flags]

Flags:
  -f, --freshen         Freshen price database
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

**Source code**: [`tools/getQuotes`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getQuotes)

