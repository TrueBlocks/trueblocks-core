---
title: Accounting for the Revolution™
lead: Is it possible?
date: '2017-03-13T03:07:38.644Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 989
---

In recent weeks, the price of ether has risen from around $10.00 US dollars per ether to hovering around $20.00 US in recent days. Needless to say, this has caused a lot of [discussion](https://www.reddit.com/r/ethereum/comments/5yxlax/any_reason_why_we_shouldnt_half_the_gas_price_to/).

We wondered if this increase in price meant that the Ethereum community was all of a sudden spending thousands of dollars more per day on gas. Or was it hundreds of dollars more per day? Or was it one dollar per day? Or even one penny? We had no basis to answer that question, so we set out to find the answer for ourselves.

Before we began our search, we imposed a self-imposed rule. This rule was that we could only get the data in a fully-decentralized way. This means we could use no external web APIs or rely on any third parties. We could only use data directly from the a locally running Ethereum node.

If you’ve ever tried to do this, you know that this means you should sit back and relax. Can you say: “slooooow”?

Over the past year, we’ve been working on speeding up this data access in a fully-decentralized manner. We call our solution TrueBlocks. The word “quick” being operative. Written in C++, TrueBlocks is able to achieve speeds more than 200 times faster than the RPC. This allows us to efficiently produce answers to these questions (and many other things).

The easiest way to explain TrueBlocks is to show you some code, and I will do that shortly, but first, I wanted to talk about…

#### Blockchain on a Stick™

TrueBlocks scrapes, parses, pre-digests, and caches the entire Ethereum blockchain in a fully-decentralized manner. We make the claim that our work is so decentralized that we can store the entire chain — fully parsed and heavily optimized — on an external drive. We call this “Blockchain on a Stick™,” and we carry it around with us in our backpack wherever we go.

![](/blog/img/011-Accounting-for-the-Revolution-001.png)

BlockChain on a Stick™ allows us to prove to potential clients that our solution is decentralized. We can, if we wish, disconnect from the Internet, and while TrueBlocks cannot access the latest blocks, it still works perfectly. That’s decentralized.

TrueBlocks uses the node’s RPC in the same way that `web3.js` does. After pre-processing the received data, we store each block, every transaction, every receipt, and every log in a local database. Before storing the data, we optimize the crap out of it in every way we can think. Our goal is this: quick!

We retrieve each block and then, if it has transactions, we spin through each transaction asking for the transaction’s receipt. Given the receipt, we make a determination if the transaction needs to be traced. We trace transactions that may have finished in error. Additionally, we identify internal transactions that were initiated by the transaction and parse the input data field and the receipt’s logs.

We do all of this pre-processing prior to storing the data, and because the blockchain is immutable, we only need to do this once. (This is not technically true because of chain re-organizations, but we won’t complicate matters by explaining how we handle this.) Every time we access the data thereafter, we are reading highly-optimized data intended specifically to be retrieved quickly.

Another thing TrueBlocks does while storing the data is to store various levels of detail. (Yes. It uses more disc space, but it’s **_significantly_** faster.) This allows the developer to choose between higher speed given less detail verses lower speed if more detail is required. It’s the programmer’s choice. For the analysis below we chose the half-speed / half-detail version.

#### Programming TrueBlocks™

TrueBlocks is a C++ library and a series of applications. Below we show you the application code we wrote that gathered the data we needed to do our analysis. Below is the actual code. It’s pretty simple. As with all C++ code, we start with the `main` function:

![](/blog/img/011-Accounting-for-the-Revolution-002.png)

This function first initializes the TrueBlocks library and then decides on the start block and the number of blocks to visit. We chose to start at block 2,912,407 (the closest block to January 1, 2017). The function `getLatestBlock` returns the node’s latest block.

Next, we create a data structure that will store the result of visiting each block. This structure may be of any type — whatever is appropriate for your application. The code then calls into the TrueBlocks library function `forEveryNonEmptyBlockOnDisc` and finishes by making a final report.

TrueBlocks has a number of interfaces for traversing blocks. For our purposes, we used the middle-of-the-road `forEveryNonEmptyBlockOnDisc`. This function gives full detail, but skips over blocks with no transactions (about 37% of all blocks).

A similar function called `forEveryBlock` visits every block (including empty ones) in full detail. On the other end of the spectrum is the function `forEveryMiniBlockInMemory` which is super-fast but delivers a lot less data than the other methods. The function one uses depends on ones application.

#### Visiting Every Non-Empty Block

If you’re familiar with C++, you’ve noticed that the bulk of the work is completed in the function `visitBlock`. Here is that function:

![](/blog/img/011-Accounting-for-the-Revolution-003.png)

At each non-empty block, the pointer to the arbitrary data is retrieved. In this case, it’s a pointer to the instance of `CGasTracker` from `main`.

The function first figures out when the block occurred. The function then determines if this is a new day. If it is, a report on the previous day is made. In this way, we accumulate and report on statistics once per day (see the data tables below).

The function then grabs the price of ether in US dollars at the time of this block, that is, it grabs the block’s spot price. TrueBlocks gets this data from the Poloniex price API. (Okay — we admit it — we broke our own self-imposed rule! But we cache the price data so it still works when unplugged.)

The function then spins through each transaction and prices the gas consumed by that transaction. Note, that we do not distinguish between in-error and successful transactions. Even if the transaction ended in error, the gas was expended, so we want to account for it.

`CGasTracker` simply accumulates the data each day, holds the Ethereum price data, and reports the results.

#### How Much More is the Community Paying for Gas?

What effect has the recent price increase had on the Ethereum community as a whole? The chart below shows the amount of Ether and US dollars spent each day during January of 2017:

![](/blog/img/011-Accounting-for-the-Revolution-004.png)

On average, the Ethereum community spent slightly more than 34 ether per day during the month of January. Most of that month, the spot price hovered around $10.00 US per ether, it figures, then, that the average number of US dollars spent per day on gas was about $340 US dollars. Looking at it from the perspective of an average transaction, one can see that this translates to between 7/10 of a penny and 8/10 of a penny per transaction. (Note: we use finneys and cents as opposed to ethers and dollars because the numbers are too small otherwise. [A finney is 1/1000 of an ether.](https://medium.com/@tjayrush/what-the-f-is-a-finney-8e727f29e77f#.u0qs5u9iq))

This seemed low, but we checked it against EtherScan and [this posting from Bok](http://ethereum.stackexchange.com/questions/11160/how-many-transactions-are-executed-on-the-ethereum-network-per-day-and-what-are/11161#11161), and our code reports identical numbers. (Yes, we know, more external data — but this time we’re just double checking.) Over the entire month of January, the community spent nearly $10,700 US dollars on gas.

How about February?

![](/blog/img/011-Accounting-for-the-Revolution-005.png)

Three things are changing here:

1. The number of transactions per day (50,359) rose by about 10.1% over January. More people made more transactions.
2. The amount of ether spent per day (on average) rose by more than 10 ether per day (or a little over 30%). Part of that is due, obviously, to more transactions per day, but also the gasPrice in wei may have risen.
3. The amount of US dollars spent per day on average rose by nearly $200 dollars (57.97%) per day. Clearly, this has to do with both of the previous things plus the increased price of ether.

It’s hard to untangle these three components, but you can get a glimpse of what’s going on by focusing on the `Finneys` and `cents` columns. While we leave it to the reader as an exercise to figure out why on the 13th, the 15th, and the 22nd finneys per transaction rose above 1.0000, one can see that the average number of finneys per transaction over the month remained relatively stable. In other words, the price (in ether) that miners accepted to process transactions didn’t adjust too much even though the amount of total income taken by all miners ($15,250.26 vs. $10,688.16) increased by nearly 42.68%.

One may see the price in US dollars increasing per transaction in the `cents` column. At the start of the month, the price per transaction in cents was around 0.9 pennies. By the end of the month, the price in cents had risen to around 1.3 pennies per transaction.

So what happened in March? Obviously, the expectation is that we will see an even more profound effect on the per transaction cost given the skyrocketing price of ether this month (the last day we analyzed was Saturday, March 11th).

![](/blog/img/011-Accounting-for-the-Revolution-006.png)

Here we see an even more marked rise in the ‘penny’ price of a transaction. At the start of January, a transaction cost on average about 1/2 of one penny per transactions. Today, the price per transaction (in US dollars) is three times higher. The average price per transaction in finneys rises over the same time period, but not by three times over. This makes sense given the significant increase in the price of ether.

#### Let’s Look at a Chart

![](/blog/img/011-Accounting-for-the-Revolution-007.png)

I think you can see from the chart that through the month of January and into mid-February the price of a transaction in both ether (finneys) and dollars (cents) tracked each other quite closely.

Near the end of February, we see the prices starting to diverge. We at TrueBlocks believe this is because the price of ether per US dollar changes much faster than the gasPrice miners are willing to accept to process a transaction. Furthermore, while both Parity and Mist allow the user to adjust the provided gasPrice, we don’t believe their default gasPrice changes effectively (although what the word ‘effectively’ means here is hard to say).

One can see the cost of gas per transaction in finneys lowering in recent days, but that price is not lowering as quickly as the price of ether vs. US dollar is rising, thus the divergence.

I am tempted to say that this implies an imperfect market, but I think that would be a mistake. If one looks at the gasPrice market (that is the price miners will accept to process transactions) in terms of US dollars, then it does seem in-efficient. But I think one must view the market from the perspective of the “realm of ether.”

One would think that, if it were a perfect market, some miners would accept lower gas prices (because ether is worth more), and therefore the price of a transaction in ether would lessen as quickly as the price of ether per dollar rises, but this is not happening.

I live in America. I use US dollars every day. I don’t use ether every day. When I think of ether, I automatically translate it into US dollars. But if I enter into a “realm of ether” mind set — if I try to force myself into thinking about ether only — then I can see that there is no real intrinsic reason for it to change simply because the price per ether changes. Certainly there is no reason for it to change as fast as the price of ether per US dollars.

#### Conclusion

We are developing TrueBlocks because we want to Account for the Revolution™. We have a thousand ideas of what that might mean. Other people have done this sort of analysis before, however, many of those analyses suffer from one of two problems: (1) they are slow, or (2) the are centralized. TrueBlocks is both fast and fully decentralized.

We know of an analysis that took more than 20 hours to scan just 15 days worth of DAO related data. (We confirmed this on our own machines). TrueBlocks ran through the above 70 days of data in less than four minutes. 20 hours for 15 days vs. 4 minutes for 70 days. Them is some quick blocks!

_If you like our work, please support us by sending a few ether (or parts of an ether) to our tip jar: 0xf503017d7baf7fbc0fff7492b751025c6a78179b. We offer all types of blockchain consulting, especially analysis and accounting services. Please find contact information on our website: <http://trueblocks.io>._
