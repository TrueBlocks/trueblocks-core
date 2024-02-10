<!-- markdownlint-disable MD012 MD034 -->
The primary tool of TrueBlocks is `chifra export`. This tool extracts, directly from the chain,
entire transactional histories for one or more addresses and presents that information for use
outside the blockchain. The results of this extraction is stored in a data structure called a
[Monitor](/data-model/accounts/#monitor).

Monitors collect together [Appearances](/data-model/accounts/#appearance) (`blknum.tx_id` pairs)
along with additional information such as [Reconciliations](/data-model/accounts/#reconciliation)
(18-decimal place accurate accounting for each asset transfer), [Names](/data-model/accounts/#names)
(associations of human-readable names with addresses), and [Abis](/data-model/accounts/#abis)
which track the "meaning" of each transaction through its [Functions](/data-model/accounts/#function)
and [Parameters](/data-model/accounts/#parameters).

Each data structure is created by one or more tools which are detailed below.
