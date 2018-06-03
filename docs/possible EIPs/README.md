## Possible EIPs

This is a list of very rudimentary ideas for possible EIPs intended to make the 'health of the archive' better. That is, these EIPs would make Etheruem address auditing easier.

1. An option to the node called something like `--first-sync-block` which takes a block hash as an option and begins syncing only at that block. This would be useful for auditors who want a full, archive node, so they can audit fully an address or collection of addresses, but wouldn't require the entire blockhain history to be downloaded to the local hard drive. This is no worse than the current situation, in fact, it would be much better. Currently, if one wants to get a full transaciton history for a given account, one must use a third party scraper such as EtherScan. There is no less trust involved here because the end user is speficify the block hash of where to start the sync.


2. An option something along the lines of `eth_clearHistory(blockNum)` which would delete everything past the given block. This would be useful if one has already extracted all the data one needs for auditing for a particular address. For example, if one is only building an audit node for the ENS smart contract, the first thing that is true is that blocks prior to the ENS's deployment are not needed. The other obseravation is that once the historical data is retrieved, there is no need for the node to store those old blocks any longer. Yes--it's wasteful to have a node devoted to auditing only a single smart contract, but that doesn't mean it's not useful to be able to do that. This "keep the duplicated data" design principal is what will allow the system to remain fully decentralized.


3. An RPC function called something like `eth_getUniqueAddressesPerBlock(blockNum, options)` which would return a list of addresses involved anywhere in the given block. This would allow end user to more easily build thier own indexes of transactions per address, but would not require the node to keep an index of transactions per address on its own. An alternative (and easier) RPC method would be `eth_getAddressesPerBlock(blockNum, options)`. `Options` to the function might be:

    - `toFrom` which would only return addresses found in the `to` or `from` field of the top level transactions.
    - `toFromTrace` which would return addresses with `to` or `from` at the top level or any trace call
    - `logs` which would return addresses appearing as the log address
    - `create` which would return smart contract address created
    - `selfdestruct` which would send addresses that self-destructed
    - `data` which would return addresses used as data in other smart contracts, or
    - `all` which would return every address involved in any way.

4. `enhanced blooms`: A new RPC call `eth_EnhanceAdaptiveBloom(blockNum, options)` that would return an enhanced adaptive bloom which is described in the [white paper](../../src/other/papers). This could be easily stored off-chain and permit significantly faster scanning of the chain for addresses of interest. `Options` might be similar to above.


5. Get `geth` and `parity` to agree on the `trace_` RPC commands.


### Other Ideas About Audting

There should be a repository of test cases where a list of transactions for particular accounts are records. The format of these lists should be as small as possible (i.e. blockNumber.transactionID as opposed to transaction hash). These lists of transactions must include all incoming and outgoing internal transactions as well as any use of an address as data in a smart contract. They may contain false positives, but should avoid false negatives. Note: it is not impossible, but very time consuming, to provide a list of all uses of an address as data. For example, if an address is used as data in a packed byte array (as oppossed to with leading zeros), it's very hard to find.

There should also be test smart contracts that are speficially designed to be hard to audit (i.e. use calls into calls to send previously stored ether or tokens from one place to another).

