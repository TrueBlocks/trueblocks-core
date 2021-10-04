The primary purpose of TrueBlocks is to extract, directly from the blockchain, the entire transactional history for one or more addresses and present that information for use outside the blockchain. The results of this extraction are stored in a data structure called a [Monitor](/data-model/accounts/#monitor).

Monitors collect together [Appearances](/data-model/accounts/#appearance) (`blknum.tx_id` pairs) along with additional information such as [Reconciliations](/data-model/accounts/#reconciliation) (18-decimal place accurate accounting for each asset transfer), [Names](/data-model/accounts/#names) (associations of human-readable names with addresses), and [Abis](/data-model/accounts/#abis) which track the "meaning" of each transaction through its [Functions](/data-model/accounts/#function) and [Parameters](/data-model/accounts/#parameters).

_Each data structure is created by one or more tools which are detailed below_
