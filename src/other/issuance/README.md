## Modeling and Auditing the Ethereum Supply

This is an attempt to respond to #SupplyGate, which was a silly Twitter discussion about the fact that getting Ethereum's current supply is not as easy as it should be. News flash: it's not.

We wrote two articles (here and here) trying to explain the calcuations. We did this mostly to help ourselves understand how to write the code in this repo.

Here's links to the two articles which discuss:

Ethereum Issuance: minerReward
Ethereum Issuance: uncleReward

### Building the Code

First, you must download, build and get running the TrueBlocks core libraries and tools.

After that, follow these instructions to build this repo:

    cd ./src/other/issuance
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

### Creating the Data

#### List of Uncle Blocks

First, we'll show you how to create the data we use in the data anaylsis article mentioned above. Follow these instructions from the `./src/other/issuance` folder:

    mkdir data
    bin/issuance --uncles | tee data/uncle_blocks.csv

This takes a while to run, but it's worth it, as it will save a lot of time in later processes.

This command spins through each block calling `getUncleCount` on each block. If a block has uncles, it writes the `blockNumber` to the output. We save the results in the file `./data/uncle_blocks.csv`. The next process will use this data file.

#### Modeling the issuance

The next process, which uses the file `data/uncle_blocks.csv` created in the previous step, generates for each block the 'expected' accounting for the blockReward, the nephewReward, and the uncleReward for each block.
