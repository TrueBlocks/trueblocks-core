## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

### Usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data. This tool has been deprecated.

`Where:`

|          | Option                           | Description                                                                                    |
| -------- | -------------------------------- | ---------------------------------------------------------------------------------------------- |
| &#8208;f | &#8208;&#8208;freshen            | Freshen price database                                                                         |
| &#8208;p | &#8208;&#8208;period &lt;val&gt; | increment of display, one of [5, 15, 30, 60, 120, 240,<br/>1440, 10080, hourly, daily, weekly] |
| &#8208;a | &#8208;&#8208;pair string        | which price pair to freshen or list (see Poloniex)                                             |
| &#8208;e | &#8208;&#8208;feed &lt;val&gt;   | the feed for the price data, one of [poloniex, maker,<br/>tellor]                              |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                                              |
| &#8208;v | &#8208;&#8208;verbose            | set verbose level (optional level defaults to 1)                                               |
| &#8208;h | &#8208;&#8208;help               | display this help screen                                                                       |

`Notes:`

- Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

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
