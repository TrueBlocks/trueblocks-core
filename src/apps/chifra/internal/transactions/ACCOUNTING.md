# Near-Perfect, Every-Block, 18-Decimal-Place-Accurate Accounting for EVM-Based Blockchains

In this document, we articulate the accounting aspects of the TrueBlocks system. While, in the title above, we are not able to say "perfect every-block accounting," we are able to say "near-perfect," and we could have said "more-perfect than any other system we know of."

The driving intent of our system is to answer the question, "Why can't I do perfect off-chain accounting on a blockchain if the on-chain accounting is perfect?" with a response of, "You can and here's how." We've described our answer in detail below starting with a definition of terms. Following the description of our system, we discuss why answering this seemingly simple question: "why can't I do perfect off-chain accounting" is much more difficult than it seems.

## Definitions

In this section, we will define the terms we use in the following section where we detail our solution.

### Address
- Any ethereum address including both smart contracts accounts and externally owned accounts (EOA).
- *Suggestion:* Stop calling them EOAs. It means nothing to anyone other than core devs. We should call them "user's accounts" (and we do so in the following document).
- An address is the fundamental entity of EVM-based blockchains. Addresses hold balances of either raw ETH or ERC20 tokens (of all forms).

### Appearance
- For any given address, an appearance is a `<bn.txid>` where `bn` is the block number and `txid` is the transaction index of where in the blockchain the address appears. An address may "appear" many times on the blockchain.
- Appearances happen in obvious places such as the `from` and `to` addresses of a transaction, or in much more esoteric places such as inside of `logs` and even deep inside a trace many layers down the call stack of a single transaction.
- If an address appears more than once in a transaction we only record the first such appearance. We call this a "unique appearance."
- Appearances point to transactions, but unless one digs deeply as described in XXXX, finding the address in question may not be easy.
- The definition of appearance is purposefuly as inclusive as possible. This inclusivness is necessary to ensure an account's history reconciles.
- The document Specification for the Unchained Index details the process of creating an index (i.e. a list) of every appearance of every address anywhere on the chain. This index is required to complete a fully-reconciled accounting for any address.

### Account History
- An account history is the list of unique appearances for any given address.
- Without a complete and accurate account history, it is not possible to deliver a reconciled accounting of balance (or any state) changes.

### ERC20 Token
- Any smart contract that supports the ERC20 standard.

### ERC20 Token Derivation
- Any smart contract that supports the ERC20 standard or any derivative of the ERC20 standard such as the ERC721 standard.

### Asset
- An asset is a smart contract that, during any point in its history, responds with a non-zero value when queried for `totalSupply` or `getBalance`.
- An asset may or may not be an ERC20 (or a derivative) token.
- Smart contracts that are not ERC20 complient may still be labeled an asset. This is purposeful in order to rather more inclusive than less.
- Any smart contract complient with the ERC20 standard (or any derivitive) is automatically included.
- Note that we purosefully do not denote an address as an asset by virtue of generating Transfer logs as it is possible for a smart contract to generate Transfer logs without a corresponding balance change. Only smart contracts that exhibit true balance transfers are considered assets.

### Block
- A list of transactions included in the canonical fork of a given blockchain.
- Blocks containing no transactions are still valid blocks and may include unique address appearances that exhibit balance changes and therefore must be accounting for. For example, mining rewards.
- A block may or may not include a block reward.
- The amount of the block reward for a given blockchain may change over time.
- A block may or may not (depending on the blockchain) contain a `miner` or `proposer` address.
- Blocks may or may not contain miners of `uncle` blocks.

### Transaction
- For the purposes of our system, a transaction must be included in a "cannonicla" block to be called a transaction. (This purposefully excludes pending transactions or transactions in "unripe" blocks.)
- Definition of an "unripe" block is configurable, but in our default system it is a block less than 28 blocks deep (or about six minutes).
- Every transaction contains at least one (and frequently many more) *traces*
- Many transactions (but no-where near all) contain one or more logs.
- Transactions will always contain at least one unique address appearance. This is because for any transaction to be initiated, it must be initiated from an EOA (User's account). This address will appear in the `from` field of the transaction.

### Reciept
- Every transaction has exactly one reciept.
- Receipts store certain information including gas usage and the creation of new smart contracts and also a possibly empty list of logs (or events) generated during the execution of the transaction.
- A transaction that has not yet been included in a block (and is therefore not included in our accounting -- i.e., a pending transaction) will have no events.

### Logs (Events)
- A log (or event) is the programatic representation of "something that happened" during the execution of a transaction.
- Logs appear as a possibly empty array in the transaction's receipt.
- There may be any number of logs in a given receipt (including zero).
- Logs may be generated at any level of the execution trace of a single transaction including many levels deep in the deepest trace.
- When doing a reconciliation, not every log in a given transaction will be `relevant` to the account being reconciled. For example, during an airdrop, balences may change for many addresses, but only the log denoting the balance change for the address currently being reconciled is `relevant`.

### Log Topic
- Every log has at least one and possibly more "topics." Topic0 (topic zero) is a representation of the Event Signature of the log. For example, for topic `0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef` represents the event signature `Transfer(address,address,uint256)`.
- Topics may be used to filter logs when querying the node, which may be massive otherwise.

### Trace
- A trace is the lowest unit of processing reported by the RPC.
- Traces of the type required by our system (colloquially called "Parity traces" or sometimes the `trace_` namespace) are only available from some RPCs.
- Some versions of the node software do provide the `trace_` namespace, but their implementation is either not consistent nor fast enough to be practicle.
- Many remote RPC providers do not provide support `trace_` at all. Some charge high fees for these endpoints.
- We very highly recommend running your own local version of Erigon which provides excellent support for `trace_` and is very fast.
- Traces appear as an array of one or more items in each transaction.
- The first trace in the list is identical (modulo a few small gas details) to the transaction itself.
- A trace consists of three parts. The `Trace` itself. A `TraceAction` (which records the "intention" of the execution) and a `TraceResult` that records the result of the execution.
- Asset transfers happen during the execution of traces (but are frequently signaled in an *log*).
- Traces are expensive to query and should be avoided when possible. Logs are much faster to query, but may be inadequate to fully reconcile a given transaction. Our system tries to reconcile using logs only and reverts to traces only when necessary.
- Traces should only be queried when absolutely necessary.

### Transfer
- A transfer is a *trace* or *log* that indicates a change in balance of asset for a given address at a given appearance.
- Transfers may happen between any two addresses.
- There are four types of transfers that our system accounts for.
  - Direct transfers of ETH, 
  - ERC20 (and derivitive) transfers,
  - Systemic transfers, and
  - Implied transfers as detailed below.

### Direct Transfers of ETH
- In our parlance, a direct transfer is the transfer of the ETH token. In this case, the transfers is from the account balance of one address to another. This is recorded in the `value` of the transaction or the `amount` feild of the `TraceAction` (and because the first trace IS the transaction, equivently in the transaction).
- A log (or event) may be, but very frequently is not, generated to note such a transfer. For example, when an EOA sends ETH to another EOA, no events are generated.
- ETH transfers are denoted to 18-decimals places (called WEI).

### ERC20 Transfers
- ERC20 transfers (even those generated by smart contracts that are derivations or enhancements to ERC20 tokens) are recorded in logs with the topic for for the event signature of `Transfer(address,address,uint256)` (see above).
- If a log has the `Transfer` topic, and the address being reconciled appears in this log (and there is an actual transfer of the ERC20 token balance as revealed by `getBalance`) then this log signals an `ERC20 Transfer`).

### Systemic Transfers
- There are multiple places in the history of an account where balances change due to to some operation of the system itself occur outside of a transaction. For example:
  - Mining rewards
  - Uncle rewards
  - Slashing / staking rewards
  - Reception of value from self-destructs
  - Exceptional state changes (very rare)
    - Genesis block
    - DAO Fork
  
### Implied Transfers
- Because programmable blockchains are Turing Complete, it is not possible to enforce rules on smart contracts that preclude them from behaving in any way they wish. This is in the nature of programming languages.
- In some rare cases, the above three rules are not sufficient to account for all of the transfers that occur for an account, such as:
  - Token transfers without a corresponding event generation
  - Airdrops and token mints without a corresponding event generation
  - Accumulation over time of compounded interest in certain smart contracts that is calculated only on subsequent transactions
  - Other unknown reasons why balances change
- In these cases, the system generates a "correct entry" to notate the difference between the actual balance and the balance that would have been calculated by the system. This makes explicit any "implied" transfers.

---
The above definitions describe the more general aspects of the system and some of the data structures invovled. In the follow few definitions, we desribe concepts needed for completing the reconciliation task.

### General ideas for Balances
- All "balances" are taken a given block for a given asset and a given holding address. In other words, "my balance of ETH at block 12" or "your balance of token X at block 100."
- A node's RPC endpoints provide balances only at the block level and only after execution.
- Balances are not provided at the transaction level. Balances after single transactions in a block are not available from the RPC.
- A single address may (and frequenlty does) change balances multiple times in a single block.

### Beginning Balance
- The balance of a given asset for a given address prior to the processing of a given block. This is equvilent to the balance of the asset at the end of the previous block.
- Note that balances are only available from the chain per block.

### Ending Balance
- The balance of a given asset for a given address at the end of a given block.
- Note that balances are only available from the chain per block.

### Next Begining Balance
- The balance of the account at the start of the next block (identical to *Ending Balance* for this block)
- Useful only for explanation.

### Ending Balance at Last Appearance (EBLA)
- Distinqished from ending balance (which is the balance at the end of the previous block relative to an appearance), this is the balance of the asset for the given address at the end of that address's last known appearance which may be many blocks in the past.
- Note that the ending balance at the last appearance may be many blocks in the past or even a transaction within the same block as the given appearance.
- This balance is used when creating a reconciliation in date-ascending order. The *begining balance* for a given appearance must be the same as the *ending balance at last appearance*, otherwise the history does not reconcile.

### Beginning Balance at Next Appearance (BBNA)
- Distinquished from *begining balance* which is the asset balance of an account prior to the execution of the block, this value is the balance of an account before the start of its next appearance.
- It is useful when producing a reconciled history in date-reversed order.
- This value is also useful when doing reconciliations in date-ascending order to simulate intra-block balance histories.

### Intra-Block Balance Histories
- Because balances are only available at the end of a block, one must "simulate" intra-block account balances for an account that has mulitple transactions in the same block.
- This somewhat complicated structure is needed to produce a reconciled history due to the fact that balances are only available at the end of a block.
- There are four cases for any given appearance in a block:
  - This appearance is the only appearance in the account's history in this block:
    - EBLA: ending balance at previous block
    - BBNA: ending balance at this block
  - There are more than one transactions in a block and the transaction is the first in the block,
    - EBLA: ending balance at previous block
    - BBNA: ending balance at previous block + change during this transaction (simulated)
  - There are more than one transactions in a block and the transaction is the last in the block,
    - EBLA: ending balance at previous block + change during this transaction (simulated)
    - BBNA: ending balance at this block
  - There are more than one transactions in a block and the transaction is neither first nor last
    - EBLA: ending balance at previous block + accumulated change during all previous transactions in this block (simulated)
    - BBNA: ending balance at previous block + accumulated change during all previous transactions in this block + change during this transaction (simulated)

### Statement
- A profit and loss statement for a single address and a single asset at a single transaction
- Consists of the beginning balance of the account in the asset, the changes that occurred during the transaciton and the ending balance at the end of the appearance. (This is reqular accounting ideas.)

### Reconcilation
- one or more per-transaction reconciliations

### Reconciled Account History
- A collection of one or more statements--one for each appearance--for a given address.
- Reconciled account history includes statements for all assets in the addresses list of appearances.

## Concepts:

- The once reconciled, always reconciled principle: because transaction histories on a blockchain are immutable, and account's history may be reconciled once and never need to be reconciled again.  This is true even if the user changes the name of an account or token, because the history is immutable.
- If the reconciled histories were stored on an immutible store such as IPFS and the hash of the history was stored on chain, then this document could be used to determine if the history had been reconciled before.  This would be a good way to avoid having to read the history from the blockchain again.

## How to Reconcile an Account History:

- collect a complete and accurate list of all appearances (see the Specification for Unchained Index)
- for each appearance in the address's history:
  - if the reconcilation for this appearance has already been cached, then read it and return it
	- `readReconciliationFromCache`:
	    - clear the statements array in the transaction
        - if the cache item does not exist, return `false`
          - note that only fully reconciled appearances are cached
        - read the cache item into the statements array 
          - read the file
          - if the names database is newer than the cache item
            - update the asset's name
            - re-write the cache
          - return `true`
  - otherwise, create a list of all the transfers in this appearance
    - `getTransfers`:
      - using `timestamp`, `date` and `encoding` from the transaction...
      - using `accountedFor` from the context...
      - using `assetFilter` from the context...
      - for every `log` in the transaction's `receipt`:
        - if the `log's` `address` (i.e., the asset) does not pass the `assetFilter`, then
          - skip this log
        - if the `log's` `topic[0]` is not a known ERC20 token transfer, then
          - skip this log
        - `sender` = `log.topic[1]`
        - `recipient` = `log.topic[2]`
        - `amount` = `log.data`
        - `ofInterest` = `sender` == `accountedFor` || `recipient` == `accountedFor`
        - if `amount` is zero or not `ofInterest`, then 
          - skip this log
        - this is a non-zero valued transfer of interest and must be accounted for
        - load the asset's name and symbol from the names database (if found)
        - assign `timestamp`, `date`, `encoding` and other values to the transfer
        - add the transfer to the list of transfers
        - continue until no more logs
      - sort the transfers by `blockNumber`, `transactionIndex`, `logIndex`, and `assetAddress`
      - insert a `Transfer` representing the transfer of any ETH at the head of the list
      - return the list of transfers
    - We now have a list of all transfers in this transaction that must be accounted for for this address
    - for each transfer:
      - using a seperate ledger for each asset in this account's history
      - if we've seen this asset before then
        - note that asset's previous balance because we will need it to reconcile this transfer
      - else
        - note the asset's previous balance using the previous block's balance (which should be zero)
      - at this point we have previous balance for this asset
      - reconcile the flow of funds within the transaction using higher level transactional and log data
      - `reconcileFlows(transfer)`:
        - using `sender` and `recipient` from the transfer...
        - if the asset is ETH:
          - if the transactions `from` is `accountedFor`, then
            - `gasOut` = `recipt.getUsed` * `gasPrice`
          - if `sender` is `accountedFor`, then // `sender` and `from` may be different if this ETH transfer is from a trace
            - `amountOut` = the transaction's value if it did not end in an error
          - if `recipient` is `accountedFor`, then
            - recipient may be
            - `0xPrefund`:
              - `prefundIn` = the transaction's value
            - `0xBlockReward`:
              - `minerBaseRewardIn` = the base miner reward
              - `minerNephewRewardIn` = the nephew miner reward
              - `minerTxFeeIn` = the transaction fee
            - `0xUncleReward`:
              - `minerUncleRewardIn` = the uncle miner reward
            - `amountIn` = the transaction's value if it did not end in an error
          - Note that multiple of the above may be true at the same time
        - else if the asset is not an ETH transfer:
          - if `sender` is `accountedFor`, then
            - `amountOut` = the transfer's value if it did not end in an error
          - if `recipient` is `accountedFor`, then
            - `amountIn` = the transfer's value if it did not end in an error
          - if both are not true, there's an error in the calc as all transfers must have a sender or recipient that is `accountedFor`
        - `begBal` = the asset's balance before this transfer
        - `endBal` = the asset's balance after this transfer
        - if the reconciliation balances, then
          - return `true`
        - else
          - we've failed to reconcile using top level transactional and log data, we must descend into the trace data:
          - this is a last resort, we should not get here often
          - if this fails, we will create an implied transfer
          - we only decend into the trace data if the asset is ETH (why is this?)
          - `reconcileFlows_traces`:
            - reset all accounting values other than `gasOut` which is always correct at the receipt level
            - handle the special case of the `prefundIn` transfer
            - query for traces for this transaction
            - for each trace (note that there may be zero):
              - if it's a refund from a self destruct, make note of incoming ETH
              - if it is a smart contract that self destructed, make note of outgoing ETH
              - both of the above may be true
              - handle some weird cases where smart contracts may have bugs in them
              - handle miner and uncle miner rewards
            - if the transaction is reconciled, return true
            - else, we've failed to reconcile using trace data, we must create an implied transfer
        - we've concluded reconciling flows internal to the transaction itself (including possibly descending into trace data and possibly creating an implied transfer. We must now balance accross transactions.
        - `reconcileBalances`:
          - using `prevDifferent` and `nextDifferent` from the context...
          - each are `false` if the appearance is in the same block either after or before respectively.
          - BEOLB: balance of the asset at the end of the last block
          - BEOB: balance of the asset at the end of the block
          - begBalOut = BEOLB
          - endBalOut = BEOB
          - four cases:
            - block zero: balances are zero
            - both prev and next are a different block: use the balances
            - prev is different: use the begBal from the end of the last block and calculate the endBal
            - next is different: use the calculated beginning balance and the ending balance
            - neither is different: use the calcualted values
          - assign reconciliation type
        - The reconciliation should be finished now however, some transfers are 'phony'. (i.e., an event but no balance change). Correct for those.
    - after all of this work, the appearance should be reconciled. All that's left is to price it using Uniswap.

### Accounting for Other Assets
- There are, of course, methods of representing assets other than ERC20 tokens and ETH balances. All of these other methods, given that they are non-standard, are ignored by our system.
- The software is open source, however, and we welcome contributions to the code base.
