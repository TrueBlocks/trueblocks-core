---
title: "Other"
description: ""
lead: ""
date: 2021-09-19T01:48:55
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1900
toc: true
---
These commands call some useful miscellaneous tools:
* `chifra quotes`, a rudimentary pricing tool,
* `chifra explore` a quick way to open a blockchain explorer,
* `ethslurp` an older tool that lets you call data from EtherScan. (This has issues of ceentralization and data quality, see explanation in its section).

{{< alert icon="👉" title="note about keys"
text="Note: some of these tools, like `ethslurp`, require an EtherScan key." >}}
[Follow these instructions to add a key to your config](https://docs.trueblocks.io/docs/prologue/installing-trueblocks/#3-update-the-configs-for-your-rpc-and-api-keys)

## chifra quotes

The `chifra quotes` tool provides USD ($) price data to other tools and to the command line. Currently, `chifra quotes` retrieves its price data from the Poloniex API. In future versions, we will add other sources and provide a mechanism for fair averaging. Ultimately, as more of this type of price data becomes available on chain, we will use those source, but this is not yet implemented.

### usage

`Usage:`    chifra quotes [-f|-p|-a|-e|-v|-h]  
`Purpose:`  Freshen and/or display Ethereum price data. This tool has been deprecated.

`Where:`

{{<td>}}
|          | Option                           | Description                                                                                    |
| -------- | -------------------------------- | ---------------------------------------------------------------------------------------------- |
| &#8208;f | &#8208;&#8208;freshen            | Freshen price database                                                                         |
| &#8208;p | &#8208;&#8208;period &lt;val&gt; | increment of display, one of [5, 15, 30, 60, 120, 240,<br/>1440, 10080, hourly, daily, weekly] |
| &#8208;a | &#8208;&#8208;pair &lt;str&gt;   | which price pair to freshen or list (see Poloniex)                                             |
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

**Source code**: [`tools/getQuotes`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getQuotes)

## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

### usage

`Usage:`    chifra explore [-l|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Open an explorer for a given address, block, or transaction.

`Where:`

{{<td>}}
|          | Option                        | Description                                                         |
| -------- | ----------------------------- | ------------------------------------------------------------------- |
|          | terms                         | one or more addresses, names, block, or transaction<br/>identifiers |
| &#8208;l | &#8208;&#8208;local           | open the local TrueBlocks explorer                                  |
| &#8208;g | &#8208;&#8208;google          | search google excluding popular blockchain explorers                |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                   |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                    |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                            |
{{</td>}}

**Source code**: [`apps/fireStorm`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/fireStorm)

## chifra slurp

`chifra slurp` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from [http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction for a given account. This is actually a shortcoming with EtherScan. It's too complicated to explain here, but see our blog.

While `chifra slurp` has its shortcomings, it does provides some nice features. You may use it to pull any transaction initiated by an EOA for example or to explore mining rewards. Visit the above referenced website for more information.

### usage

`Usage:`    chifra slurp [-t|-p|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetch data from EtherScan for any address.

`Where:`

{{<td>}}
|          | Option                          | Description                                                                                                  |
| -------- | ------------------------------- | ------------------------------------------------------------------------------------------------------------ |
|          | addrs                           | one or more addresses to slurp from Etherscan (required)                                                     |
|          | blocks                          | an optional range of blocks to slurp                                                                         |
| &#8208;t | &#8208;&#8208;types &lt;val&gt; | one or more types of transactions to request, one or<br/>more of [ext, int, token, nfts, miner, uncles, all] |
| &#8208;p | &#8208;&#8208;appearances       | show only the blocknumer.tx_id appearances of the exported<br/>transactions                                  |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;   | export format, one of [none, json, txt, csv, api]                                                            |
| &#8208;v | &#8208;&#8208;verbose           | set verbose level (optional level defaults to 1)                                                             |
| &#8208;h | &#8208;&#8208;help              | display this help screen                                                                                     |
{{</td>}}

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

**Source code**: [`tools/ethslurp`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethslurp)

