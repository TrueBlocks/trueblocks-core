getQuotes argc: 2 [1:--readme] 
getQuotes --readme 
### Usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data. This tool has been deprecated.

`Where:`

{{<td>}}
|          | Option                           | Description                                                                                    |
| -------- | -------------------------------- | ---------------------------------------------------------------------------------------------- |
| &#8208;f | &#8208;&#8208;freshen            | Freshen price database                                                                         |
| &#8208;p | &#8208;&#8208;period &lt;val&gt; | increment of display, one of [5, 15, 30, 60, 120, 240,<br/>1440, 10080, hourly, daily, weekly] |
| &#8208;a | &#8208;&#8208;pair string        | which price pair to freshen or list (see Poloniex)                                             |
| &#8208;e | &#8208;&#8208;feed &lt;val&gt;   | the feed for the price data, one of [poloniex, maker,<br/>tellor]                              |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                                              |
| &#8208;v | &#8208;&#8208;verbose            | set verbose level (optional level defaults to 1)                                               |
| &#8208;h | &#8208;&#8208;help               | display this help screen                                                                       |
{{</td>}}

`Notes:`

- Valid pairs include any pair from the public Poloniex's API here:
  https://poloniex.com/public?command=returnCurrencies.
- Due to restrictions from Poloniex, this tool retrieves only 30 days of data
  at a time. You must repeatedly run this command until the data is up-to-date.

