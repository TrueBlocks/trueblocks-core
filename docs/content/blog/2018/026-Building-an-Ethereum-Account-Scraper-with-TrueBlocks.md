---
title: Building an Ethereum Account Scraper with TrueBlocks
lead: First pass instructions
date: '2018-07-06T20:21:50.377Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 974
---

When I first heard of Ethereum, I thought to myself _“Excellent! I no longer have to keep track of my spending. Everything will be automated.”_

> **_Can you say “misconception”?_**

Not only is keeping track of Ethereum spending _way_ more difficult than it should be, it’s actually so difficult that I stopped spending ether all together about two years ago. (The price of ether had a lot to do with it as well.)

_“Why_,” I’ve been asking myself every since, “_is it so hard to keep track of my accounts? Why isn’t there an RPC command_ `_eth_getAccountTransactions_`_?”_

I’ve been working on this shortcoming ever since. I’m glad to say that, as the latest version of TrueBlocks gets us a long way towards that goal.

In this post, I will show you how to build an Ethereum account scraper. An _account scraper_ is different than a _blockchain scraper_. (Sorry for the clickbait title.) I think of a _blockchain scraper_ as a full duplicate copy of the entire blockchain stuffed into a relational database and delivered from an expensive AWS instance via an API. We’re not building that — because of the expense of that, I think it leads to centralization such as Etherscan — we’re going scrape only the parts of the chain that is of interest to ourselves. We’re going to scrape only the transactions from or to our accounts.

#### Installing TrueBlocks

Here’s [instructions for installing and building TrueBlocks](https://trueblocks.io/docs/install/install-core/). I won’t go into details. If you have trouble, please contact us through our [discord channel](https://discord.gg/RAz6DJ6xkf). Note — TrueBlocks requires a running Parity node, and that node must be a tracing node. It works against Infura, but Infura doesn’t support tracing, so you won’t get complete results. Try it though. You’ll be able to get some data even from Infura. Assuming you’re running a local Parity node…

#### Building an Account Scraper

TrueBlocks is a C++ code library to make interacting with Ethereum easier. To get started, we need to write C++’s `main` function:

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-001.png)

There are a couple of things going on here. First, you must include TrueBlocks’ header file. We do this at `line 1`. Next, we forward declare a function that will be visited each time an address is encountered in a block. We make this forward declaration at `line 4`. The function must have that signature.

At `line 8`, we use the program’s first command line argument as the number of the block we wish to visit (we use block 1,001,001 in the following examples). At `lines 9 and 10`, we query the Ethereum node for the block.

When the call to `getBlock` returns, `block` has been populated with the block data from the node. The next line, `line 11`, visits all parts of the block including each transaction, each receipt, each log, and each trace. As it visits the various parts of the block, it extracts each address it finds and calls the provided function. With each call it sends the following data:

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-002.png)

The first four data fields should are obvious. They represent where in the block the address was found. The fifth field, `reason`, details _exactly_ where the address was found. `Reason` will take on various values which are documented in the code.

At `line 19`, we send the data to the screen which produces these results:

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-003.png)

This is a list of every address in the block, its transaction position, it trace position within the transaction (if applicable) and where the address was found. (There are 15 different places where an address might appear.)

Next, we will make two changes to the code. First, we’re going to produce only a unique list of addresses (as opposed to every address encountered including dups as it currently does). Second, we will only print out addresses we’re interested in. We make the following changes to the code:

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-004.png)

We changed two things in the above code. First, we changed `line 11` to use the function `forEveryUniqueAddress` instead of `forEveryAddress`. This removes duplicates and reports only on the first appearance of an address within the block. The second change we made at `lines 20 and 21`. Here, we filter on particular addresses. This produces the following output.

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-005.png)

And now, I claim we’re done… but, let’s take it up a notch. Here’s the next version:

![](/blog/img/026-Building-an-Ethereum-Account-Scraper-with-TrueBlocks-006.png)

This takes two command line arguments: `startBlock` and `endBlock` and visits each block in the range and then visits each address in those blocks. And now, we think, you actually have a full-fledged _account scraper_. Go ahead and build you own _blockchain scraper_ if you want.

Of course, you may enhance the above code to your heart’s content. Add a lot of command line options. Add some input checking. Add feature that allows you to specify a range of blocks on the command line. Add a feature that allows you to specify the address(es) you’re interested in on the command line. Keep track of the last block visited, go to sleep every loop, and keep up with the tip of the chain. Account for the transactions as you encounter them. Export to the data to your accounting software!

Check out our command line tool called `getBlock`. This tool does much of what we describe above. `getBlock` is [fully open source](https://github.com/TrueBlocks/trueblocks-core/blob/master/src/tools/getBlock/README.md), and you can enter a command like this:

> getBlock --addrs 5000000-5250000 | grep 0x2a65aca4d5…135398226

which will scan the 250,000 blocks between 5,000,000 and 5,250,000 and filter out only transactions involving the given address. With such a list, you can do anything…including…wait for it….including…automate your accounting! Go out and spend!

#### Support My Work

I’m interested in your thoughts. Please clap for us and post your comments below. Consider supporting our work. Send us a small (or large) contribution to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

**Thomas Jay Rush** owns the software company TrueBlocks whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact him through [the website](http://trueblocks.io).
