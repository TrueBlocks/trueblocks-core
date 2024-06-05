An appearance is a pointer (`blknum, tx_id` pair) into the blockchain indicating where a
particular address appears. This includes obvious locations such as `to` or `from` as well
as esoteric locations such as deep inside a tenth-level trace or as the miner of an uncle block.
The primary goal of TrueBlocks is to identify every appearance for any address on the chain.

The TrueBlocks [index of appearances](/data-model/the-index/) (created by [chifra scrape](/chifra/admin/#chifra-scrape))
makes the production of such a list possible. Appearances are stored in [Monitors](/data-model/accounts/#monitor).
