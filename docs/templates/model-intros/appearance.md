An appearance is a pointer (`blknum, tx_id` pair) into the blockchain indicating where a particular address appears. This includes obvious locations such as `to` or `from` as well as esoteric locations such as deep inside a tenth-level trace or as the miner of an uncle block. The primary goal of TrueBlocks is to identify every appearance for any address on the chain.

The TrueBlocks [index of appearances](/data-model/the-index/) (created by [chifra scrape](/docs/chifra/admin/#chifra-scrape)) makes the production of such a list possible. Appearances are stored in [Monitors](http://localhost:1313/data-model/accounts/#monitor).

The following commands produce and manage [{PLURAL}]:

| Tools                                                 |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chifra list](/docs/chifra/accounts/#chifra-list)     | list appearances for one or more addresses                   |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
