# Modeling and Auditing the Ethereum Supply

This code is in response to [#SupplyGate](https://twitter.com/hashtag/Supplygate?src=hashtag_click) ([here](https://www.coindesk.com/how-much-ether-is-out-there-ethereum-developers-create-new-scripts-for-self-verification) and [here](https://finance.yahoo.com/news/supplygate-battle-frame-crypto-next-190000565.html)), which was a silly Twitter discussion (as most are) about the fact that a purely mathematical number, called `Total Ether Supply`, is difficult to get. News flash: it is difficult to get, especially on a desktop computer.

We wrote two articles explaining first the calcuations of the total supply by re-visting the Yellow Paper, looking closely at the stupidly complicated equations, translating those equations into English, and then writing the following code.

Here's links to the two articles:

Ethereum Issuance: minerReward
Ethereum Issuance: uncleReward

## Building and Using the Code

To get started, you must download, build and get running the TrueBlocks core libraries and tools. Do that now, and return when you've completed that task.

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

We tried to build code that runs as quickly as possible. This involves building first a list of blocks that contain uncles. With that list, we can avoid querying uncessarily for blocks that do not have uncles. The issuance for blocks that do not have uncles requires no query to the chain. The reward is a pure function of `blockNumber`.

### Listing Uncle Blocks

Assuming you've built the repo properly, follow these instructions from the `./src/other/issuance` folder to create the list of blocks with uncles:

    mkdir data
    bin/issuance --uncles | tee data/uncle_blocks.csv

This takes a while to run, but it's worth it, as it will save a lot of time in later processing.

This command spins through each block calling `getUncleCount` on each block. If a block has uncles, it writes the `blockNumber` to the output. We save the results in the file `./data/uncle_blocks.csv`. The next process will use that data file.

### Modeling the Issuance

The next process, which uses the file `data/uncle_blocks.csv` created in the previous step, generates for each block the 'expected' accounting for the blockReward, the nephewReward, and the uncleReward for each block.

Complete this process with this command:

    bin/issuance --generate

We call this process `modeling the expected value` in the articles.

This process takes a _very long time_ and creates a _very large data file_ (~ 1.1 GB) called `./data/results.csv`. It contains accounting records for each `blockReward` and all `uncleRewards`. The CSV data file has the following fields:

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

### Auditing the Reward Calcuations

Note: The processes detailed in this section requires an Ethereum archive node. A local node that allows you to query as quickly as possible is greatly preferred. Trying to hit a remote archive node such as Infura may meet the limits of the service.

To run the next process do this:

    bin/issuance --audit

The next process is to `audit` the blockchain by using the same calculations used above but, in addition, to do a `reconcilation` or `audit` at each issuances.

One way to do this would be to spin through the blocks again (or during the previous process) and `audit` each issuance as it occurs. We would do this by querying the balance of the block's `miner` and each `uncle miner` prior to the block (i.e. at `blockNumber - 1`), modeling the issuance for each beneficiary, adding that issuance to the beginning balances, and then checking the result against the balance at the end of the block.

If we've modeled it right, it should balance. Right?

News flash: it doesn't.

#### Digging Deeper

The trouble with the above process is that it ignores the fact that the miner can have mony other sources of income and/or expenditures other than the issuance in a given block. In order to account for this other spending, one must spin through all the transactions on the chain, but, it's even worse than that as not all income and expenditures are at the transaction level, so we also have to spin through every trace of every transactions all the way down to the bottom. And, now, we have a really big problem. The above process, implented nievly, would literally take months to run. We have to be more creative than that.

The goal, and the only way we've discovered to speed this process up, is to avoid querying the node at all costs -- we need to avoid querying for blocks if we can, avoid querying for transactions with attention, avoid querying for balances (although this is not possible), and especially avoid querying for traces.

As a way to avoid querying blocks so as to get the miner and uncle miner addresses, we use work we did previously in which we index every address in every block (including miners and uncle miners). This speeds up the process from many days to less than a day.
