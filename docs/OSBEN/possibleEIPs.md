## Possible EIPs

This is a list of very rudimentary ideas for possible EIPs intended to make the 'health of the archive' better. That is, these EIPs would make Ethereum address auditing easier.

1. An option to the node called something like `--first-sync-block` which takes a block hash as an option and begins syncing only at that block. This would be useful for auditors who want a full, archive node, so they can audit fully an address or collection of addresses, but wouldn't require the entire blockchain history to be downloaded to the local hard drive. This is no worse than the current situation, in fact, it would be much better. Currently, if one wants to get a full transaction history for a given account, one must use a third party scraper such as EtherScan. There is no less trust involved here because the end user is specify the block hash of where to start the sync.


2. An option something along the lines of `eth_clearHistory(blockNum)` which would delete everything prior to the given block. This would be useful if one has already extracted all the data one needs for auditing for a particular address. For example, if one is only building an audit node for the ENS smart contract, the first thing that is true is that blocks prior to the ENS's deployment are not needed. The other observation is that once the historical data is retrieved, there is no need for the node to store those old blocks any longer. Yes--it's wasteful to have a node devoted to auditing only a single smart contract, but that doesn't mean it's not useful to be able to do that. This "keep the duplicated data" design principle is what will allow the system to remain fully decentralized.


3. An RPC function called something like `eth_getUniqueAddressesPerBlock(blockNum, options)` which would return a list of addresses involved anywhere in the given block. This would allow end user to more easily build their own indexes of transactions per address, but would not require the node to keep an index of transactions per address on its own. An alternative (and easier) RPC method would be `eth_getAddressesPerBlock(blockNum, options)`. `Options` to the function might be:

    - `toFrom` which would only return addresses found in the `to` or `from` field of the top level transactions.
    - `toFromTrace` which would return addresses with `to` or `from` at the top level or any trace call
    - `logs` which would return addresses appearing as the log address
    - `create` which would return smart contract address created
    - `selfdestruct` which would send addresses that self-destructed
    - `data` which would return addresses used as data in other smart contracts, or
    - `all` which would return every address involved in any way.

4. `enhanced blooms`: A new RPC call `eth_EnhanceAdaptiveBloom(blockNum, options)` that would return an enhanced adaptive bloom which is described in the [white paper](../../src/other/papers). This could be easily stored off-chain and permit significantly faster scanning of the chain for addresses of interest. `Options` might be similar to above.


5. Get `geth` and `parity` to agree on the `trace_` RPC commands.

6. We could standardize certain test cases by purposefully do things in a smart contract that any legit blockchain scraper would need to handle properly. For example, suicides, a smart contract that calls into another smart contract and either does or does not handle a throw by throwing itself or not. A 1000 deep call stack that ends with out of gas. We can custom build transactions that describe particular behaviour (for example returning structures, returning error messages, input of arbitrarily complex ABI interfaces) and properly articulate that data. Summary (of poorly worded section): build smart contract test cases that allow parsers/scrapers/articulators test themselves.

7. You should be able to specify exactly which addresses you want to keep historical balances for. I only want the historical state for these addresses: 0x1212, 0x1221213, 0xababbaa112

8. Parity exports the first trace of every transaction as traceID zero, which contains fields identical to the fields in the top level transaction that generated the trace. The first non-top level trace is traceID 1. The trouble is that the top level transaction differs from the first (zeroth) trace only in the `gasUsed` field. So, if one wishes to store both the top level transaction and the trace level zeroth trace, one has to either (1) throw out the `gasUsed` from one of the two records, (2) store an extra column per trace showing the two different fields, (3) add another boolean flag showing top level, trace level, or (4) change the value of the traceId to stand for the level plus the position. None of these are good solutions.

9. Modify the nodes to provide hooks for extensions at certain critical places in the system. For example, after a transaction has been verified, after a block has been sealed, after a block has been confirmed, at a soft fork, at a hard fork, etc. This would allow people to build data analytical tools that are tightly integrated with the node.

10. Create a meta-data-layer inside the node that, every time a block gets finalized, it gets run (if the user has enabled the feature) and the feature does various data calculations on the block. For example, it could generate a list of addresses per block. This would be paid for with inflation as a way to incentivize people to run this optional feature and provide the data that people actually need from the node.
11. 
### Other Ideas About Auditing

There should be a repository of test cases where a list of transactions for particular accounts are records. The format of these lists should be as small as possible (i.e. blockNumber.transactionID as opposed to transaction hash). These lists of transactions must include all incoming and outgoing internal transactions as well as any use of an address as data in a smart contract. They may contain false positives, but should avoid false negatives. Note: it is not impossible, but very time consuming, to provide a list of all uses of an address as data. For example, if an address is used as data in a packed byte array (as opposed to with leading zeros), it's very hard to find.

There should also be test smart contracts that are specifically designed to be hard to audit (i.e. use calls into calls to send previously stored ether or tokens from one place to another).


