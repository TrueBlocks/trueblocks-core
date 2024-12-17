---
title: Counting Shit on Ethereum
lead: Fun with blocks, transactions, and counters
date: '2019-06-10T01:50:36.918Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 971
---

I like to count shit. I’ll count anything I can find. Cracks in the sidewalk. Pennies in a cup.

I also like to make lists. In fact, at one point in my life, combining my two passions, I was maintaining a list of over 350 rules related to how to pick up pennies without incurring bad luck.

_Yes. I suffer a bit from OCD (but happily)._

Recently, [Pedro Gomes](https://medium.com/u/a9b602b091af) made a tweet asking if anyone had a list of the most [frequently called function signatures on the Ethereum blockchain](https://twitter.com/pedrouid/status/1101194526524362752).

I thought to myself. _Oh cool. A list. I like lists. Plus, there’s counting involved. I like counting. I can do this._

To me, “I can do this”, usually translates into “I can write some code”, so here’s some pseudo code for answering Pedro’s question:

map<signature, count> counts

for (every block in blockchain)  
    for (every tx in block)  
       if (tx.input is at least four bytes long)  
           counts\[tx.signature\]++

sort the map reverse by second  
print the first twenty entries in the map

In other words, count some shit.

I work on a project called TrueBlocks, so I’ll use that to carry out the above simple process. Here’s what you would need to do to duplicate my effort:

1. Download, install, and run an Ethereum node\*
2. Download, install, and build TrueBlocks

\* — wait for a long time while the node syncs.

For more information on running node, you’ll have to look online (or checkout [DAppNode](https://medium.com/u/8d628dbdf3c2), which makes running Ethereum nodes very easy). Instructions for downloading and running TrueBlocks is [here](https://trueblocks.io/docs/install/install-core/).

#### Building the Sample Code

Before we write the code to do the counting, we want to make sure you can build the sample project. Instructions for that are [here](https://github.com/TrueBlocks/trueblocks-examples), but the short version is:

\# Download TrueBlocks  
git clone -b master https://github.com/TrueBlocks/trueblocks-core  
cd trueblocks-core  
mkdir build  
cd build  
cmake ../src  
make

\# Run Parity with --tracing on

\# From ./build, run the following command:  
../bin/getBlock 12

\# If the above command returns non-empty block data, you're set.

Now we can start writing the counting program. We start by making a copy of one of the sample programs and making sure the copy builds:

cd ../src/examples  
cp -pR simple count\_shit  
cd count\_shit  
make  
./bin/simple

You should get some data for block 3,500,000 (assuming your node is synced that far). **Note:** if you don’t want to run your own node, you can edit a file called `$CONFIG/trueblocks.toml` and set the `rpcProvider` option to a remote node such as Infura. Be warned, though, if you do you will probably get locked out due to hitting their servers too hard — this is one of the benefits of running your own node — no rate limiting.

#### A Quick Review of the Sample Code

Open the file`./samples/count_shit/simple.cpp` with your editor. It will look something like this:

![](/blog/img/029-Counting-Shit-on-Ethereum-001.png)

A few things to notice:

First, you need to `#include "etherlib.h"`. This allows you to build programs with TrueBlocks.

Second, initialize the library with `etherlib_init()`.

The real code starts at line 14. Here we create a storage location for the block data with `CBlock block` . The next line calls into the library (which in turn calls into the node) to get the block data for block 3,500,000. Line 18 sends the block data to the screen. Very simple.

#### Writing the Actual Code

Now we want to write the code to count function signatures. With little fanfare, here’s the code with short descriptions for each section.

The first section includes the `etherlib.h` header file and defines a few types that we will use to store the function signatures and then sort and report on what we find. Put this at the top of your file.

![](/blog/img/029-Counting-Shit-on-Ethereum-002.png)

The next section is the `main` routine. Put this at the very bottom of your file. We will be inserting two functions between the declarations at the top of the file and the main function. In this way, we won’t have to forward declare the functions we need.

![](/blog/img/029-Counting-Shit-on-Ethereum-003.png)

The `main` function, at line 35, declares a signature map to store the signatures, and line 36 calls the function `forEveryBlock` with the function `visitBlock` and a pointer to that map. `forEveryBlock` visits every block and calls the passed-in function on each block between the first block with a transaction `(frstTransactionBlock)` and the last available block on the chain. (We’ll look at `visitBlock` in a second.) `visitBlock` does the counting. Starting at line 38, we simply sort the gathered counts and present the top twenty results.

The remaining functions do the real work. The first, `visitBlock` gets called…

![](/blog/img/029-Counting-Shit-on-Ethereum-004.png)

…simply passing on the data pointer to every transaction in the block.

The final function, and the one that does all the work, is called `visitTransaction`.

![](/blog/img/029-Counting-Shit-on-Ethereum-005.png)

The first thing this function does is cast the data pointer back to a signature map. Some of the younger programmers who are reading this might look askance at this sort of type-safety violation, but at least one of our engineers (alright — it’s me) has been programming since 1981, so this is how we roll. It may be slightly unsafe, but it’s fast as shit.

The `visitTransaction` function checks if the `input` data of the transaction is at least four bytes long (eight characters — four nibbles) and then increments the counter at that location in the map. The remainder of the code (from line 18–21) simply reports our progress to the screen.

And that’s it. You’ve counted the frequency of function calls in the Ethereum database. One word of caution: this is slow, slow, slow, slow, slow. The RPC commands are not exactly “speed burners”.

One of the things TrueBlocks does is cache the blocks (if you want it to), so the second time you would run this code, it would be very much faster. That would come at the expense of hard drive space.

#### Conclusion

This is a purposefully simple example of the type of things you can do with TrueBlocks. Spend some time reading the code. Download and build the examples. We should all be learning how to access the Ethereum blockchain data on our own. I think having access to the chain data for oneself is an undervalued aspect of the blockchain.
