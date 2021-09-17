## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

### usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data. This tool has been deprecated.

`Where:`

{{<td>}}
|     | Option               | Description                                                                                    |
| --- | -------------------- | ---------------------------------------------------------------------------------------------- |
| -f  | --freshen            | Freshen price database                                                                         |
| -p  | --period &lt;val&gt; | increment of display, one of [5, 15, 30, 60, 120, 240,<br/>1440, 10080, hourly, daily, weekly] |
| -a  | --pair &lt;str&gt;   | which price pair to freshen or list (see Poloniex)                                             |
| -e  | --feed &lt;val&gt;   | the feed for the price data, one of [poloniex, maker,<br/>tellor]                              |
| -x  | --fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                                              |
| -v  | --verbose            | set verbose level (optional level defaults to 1)                                               |
| -h  | --help               | display this help screen                                                                       |
{{</td>}}

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here:
  https://poloniex.com/public?command=returnCurrencies.
- Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

**Source code**: [`tools/getQuotes`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getQuotes)

