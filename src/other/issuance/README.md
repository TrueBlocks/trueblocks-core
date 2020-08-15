# Modeling and Auditing the Ethereum Supply

This code is in response to [#SupplyGate](https://twitter.com/hashtag/Supplygate?src=hashtag_click) ([here](https://www.coindesk.com/how-much-ether-is-out-there-ethereum-developers-create-new-scripts-for-self-verification) and [here](https://finance.yahoo.com/news/supplygate-battle-frame-crypto-next-190000565.html)), which was a silly Twitter discussion (as most are) about the fact that a purely mathematical number, called `Total Ether Supply`, is difficult to get. News flash: it is difficult to get, especially on a desktop computer.

We wrote two articles explaining first the calcuations of the total supply by re-visting the Yellow Paper, looking closely at the stupidly complicated equations, translating those equations into English, and then writing the following code.

Here's links to the two articles:

[Ethereum Issuance: blockReward](https://medium.com/@tjayrush/ethereums-issuance-minerreward-3cad5b9a72ff)  
[Ethereum Issuance: uncleReward](https://medium.com/@tjayrush/ethereums-issuance-unclereward-72de71b0f9f6)

## Building and Using the Code

To get started, you must download, build and get running the [TrueBlocks core libraries and tools](https://github.com/Great-Hill-Corporation/trueblocks-core/blob/master/src/other/install/INSTALL.md). Do that now, and return when you're done.

Next, follow these instructions to build the code in this repo (from the ./build folder):

    cd ../src/other/issuance
    make every
    bin/issuance

The above command should return this help screen.

    Usage:    issuance [-g|-a|-u|-y|-m|-w|-d|-o|-i|-v|-h]
    Purpose:  Process various data related to Ethereum's issuance.

    Where:
        -g  (--generate)      generate expercted miner and uncle rewards for each block
        -a  (--audit)         audit miner and uncle rewards for each block
        -u  (--uncles)        generate a list of blocks containing one or more uncles
        -y  (--by_year)       summarize previously generated results by year
        -m  (--by_month)      summarize previously generated results by month
        -w  (--by_week)       summarize previously generated results by week
        -d  (--by_day)        summarize previously generated results by day
        -o  (--by_hour)       summarize previously generated results by hour
        -i  (--discrete)      while accumulating, reset accumulator at each period
        -v  (--verbose)       set verbose level
        -h  (--help)          display this help screen

    Powered by QBlocks (GHC-TrueBlocks//0.7.0-alpha-c2cc73914-20200811)

## Creating the Data

We tried to build code that runs as quickly as possible. This involves building first a list of blocks that contain uncles. With that list, we can avoid querying uncessarily for blocks that do not have uncles. The issuance for blocks that do not have uncles (nearly 9/10s of all blocks) does not require a query to the chain.

### Listing Uncle Blocks

Assuming you've built the repo properly, follow these instructions from the `./src/other/issuance` folder to create the list of blocks with uncles:

    mkdir data
    bin/issuance --uncles | tee data/uncle_blocks.csv

This takes a while to run (it has to query the chain), but it's worth it, as it will save a lot of time in later processing especially if you must re-run that later processing.

This command spins through each block calling `getUncleCount` on each block. If a block has uncles, it writes the `blockNumber` to the output. We save the results in the file `./data/uncle_blocks.csv`. The next process will uses that data file to avoid unnecessary queries to the chain.

### Modeling the Issuance

The next process, which uses the file `data/uncle_blocks.csv`, generates for each block the 'expected' accounting for the `blockReward`, the `nephewReward`, and the `uncleReward`.

Complete this process with this command:

    bin/issuance --generate | tee data/results.csv

We call this process `modeling the expected value` in the articles.

This process takes a _very long time_ and creates a _very large data file_ (~ 1.1 GB) called `./data/results.csv`, but again, it's worth it to avoid the same pain if you need to re-run later processing. It contains accounting records for each `blockReward` and all `uncleRewards`. The CSV data file has the following fields:

| Field        | Meaning                                                                 |
| ------------ | ----------------------------------------------------------------------- |
| blockNumber  | the block number of the record                                          |
| timestamp    | the timestamp of the block                                              |
| month        | the month this block was created                                        |
| day          | the month this block was created                                        |
| baseReward   | the base reward issues for all blocks ingoring uncles                   |
| nephewReward | an additionl reward given to the miner of the block for included uncles |
| blockReward  | the total reward delivered to the miner                                 |
| uncleReward  | the reward issues to the miners of the uncle blocks (if any)            |
| issuance     | the total new issuance for the block                                    |

It's important to realize that the data created in this step is a modeling of the data, not the actual data stored on the blockchain as represented by the account balances of the rewarded miners.

**Notes:** Optionally, we could have done two other things during the creation of this data (in fact, we did one of them).

1. We could have reconciled or audited the account balances during this process, but we chose not to as it would have slowed down this part of the proces unecessarily. We knew that it would be highly likely that we would be running the audit portion of the code more than once, so we decided to wait to do that until the next step, and
2. During this processing we made a small check to confirm something we suspected given our reading of the Yellow Paper. For every uncle we pulled, we checked to see if that uncle had an uncle list. Reading the Yellow Paper, we suspected it did not. We found no uncle's with a non-empty uncle lits. In other words, uncle's don't have children. In other words, uncles don't have nephews. We see _a lot of confusion_ about the word 'nephew' when discussing block rewards. Nephews are blocks that have uncles (Do you get it? A nephew has an uncle.) This is different than a nephew being the child of an uncle. Uncles do not have children. This is not semantic difference, it's important to how the reward calculation works.
3. Block zero is special. We use the baseReward field to notate the issuance during the genesis block. While this isn't technically correct (as no miner was rewarded in the genesis block), it simplifies the code.
4. After generating the `results.csv` file the first time, you may re-run the `--generate` command with the various summary options to create summarized data. This makes analizing the data easier. To show only the issuance during that period, add `--discrete`.

### Auditing the Reward Calcuations

Note: The processes detailed in this section requires an Ethereum archive node because we pull historical balances. It also requires one to have previously run the TrueBlocks `blockScrape` process to produce an index of addresses appearances. You coul run the audit process without the index, but it would literally take many weeks.

It's beneficial for the archive node to under your own control, as the process described here queries the node as quickly as possible. Trying this process against a shared archive node would probably bump up against the limits of the service.

To run the `--audit` process, first build the index [as described here](https://github.com/Great-Hill-Corporation/trueblocks-core/tree/master/src/apps/blockScrape) (or acquire it [as described here](https://hack.ethglobal.co/showcase/unchained-index-recXXu2R9nwwkhH3L)) and do this:

    bin/issuance --audit | tee data/audit_results.txt

This process uses the same calculations as above but, in addition, it also does a `reconcilation` or `audit` of each issuances.

One way to do this would have been to spin through the blocks again (or during the previous process) and `audit` each issuance as it occurs. This would require querying the balance of the winning `miner` and each `uncle miner` prior to the block (i.e. at `blockNumber - 1`), modeling the issuance as described in the aritcles, adding that modeled issuance to the beginning balances, and then checking the result against the ending balance of those accounts after the block.

If we've modeled things correctly, everything should balance. Right?

News flash: Wrong!

#### Digging Deeper

The trouble with the above process is that it ignores the fact that miners can participate in transactions in block for which they are the beneficiary. They may have other sources of income and/or expenditures during the same block. They may be both the winning miner and a participant in a transactions.

If the reconciliation of the issuance doesn't balance, one must account for this "other" spending or income by looking at all the transactions in the block until the account does balance. Unfortunately, it's actually more difficult than that. Not all income and expenditures show up at the transaction level. Many times income and/or expenditures show up only in the traces of a transactions. So, for those accounts that don't balance by looking at issuance and transactions, one must query all the traces for each transaction in the block until the account does balance.

And, now, we've encountered a really big problem.

The above process, implented nievly, would literally take months to run. We have to be more creative than that. This is why we've built our index of appearances of addresses.

The trick--and the only way we've discovered to speed processes like these up--is to avoid querying the node whenver possible: we must avoid querying for blocks if they don't contain uncles, we must avoid querying transactions if the auditing reconciles without them, we must avoid querying balances if possible (not possible in the application), and especially we must avoid querying traces.

You may ask why TrueBlocks doesn't simply extract all this data into a database once and for all using something like an ETL process. Good question. In answer, ask yourself this question: would that work on a small machine such as a desktop computer. The answer to that question is no, and since this is a requirement for TrueBlocks, you now have your answer.

As a way to avoid querying blocks so as to get the miner and uncle miner addresses, we use work we completed previously which we index of addresses created by `blockScrape`. This speeds up the process from many days to less than a day.

You can see how that works by reviewing the code.

## Upshot and Conclusion

** rant on **

The recent dustup of [#SupplyGate](https://twitter.com/hashtag/Supplygate?src=hashtag_click) was correct.

It is not easy to get an accurate number for the Ether Supply at a given block. But, it is possible as we've shown.

Part of the problems is that people call the various components of calcuation by different names: minerReward, uncleReward, nephewReward, blockReward, baseReward. Another problem is people are not careful specifying 'when' their calcuations are for (at which block). They publish a result, but they don't publish the block number for the result. Another problem we've seen trying to tie out to other work is that people have errors (including rounding errors) in their work. Another is people are _modeling_ the behavior of the equations (running code and spitting out data) but are not _auditing_ the models (few people check the actual account balances of the winning miners). Another problem is that querying the node is glacially slow. You can't re-run the results. TrueBlocks tries to fix this. Another problem is that no-one runs thier own nodes, making independant verification or auditing impossible due to restrictions or cost associated with using someone else's node.

At TrueBlocks, we've been arguing for a very long time that more people should be running more nodes. We argue further that even if when one does run a node, the ability of the node to deliver data sucks, which is why we built our indexer. We argue further that if the community is not careful, these first two problems will lead to the data being captured by rich companies such as Evil Corp. If the data is cpatured by big data corportaions, the ecosystem will be destroyed.

I fear, frequently, that the entire Ethereum community is distracted from what should be our _raison d'etre_. We should be focused on building systems that make the world a better place -- not in 1000% annual rates of return from some idiotic ponzi scheme. This lack of focus opens the community up to demogougery as per the recent 'scandles' such as [#SupplyGate](https://twitter.com/hashtag/Supplygate?src=hashtag_click).

In the end, we get what we deserve.

** rant off **
