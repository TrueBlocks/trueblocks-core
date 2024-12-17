---
title: Reading Byzantium’s Tea Leaves
lead: Louis Rukeyser looks at the blockchain
date: '2017-10-02T01:43:50.040Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 981
---

I used to watch a television show called “Wall Street Week with Louis Rukeyser.” The host would frequently laugh at the way Alan Greenspan, the Chairman of the Federal Reserve, would testify in front of Congress. Rukeyser found it comical that we all had to “read the tea leaves” after Greenspan spoke.

> “_I should warn you, if I turn out to be clear, you’ve probably misunderstood me.” ~Alan Greenspan_

Why did Mr. Greenspan speak in such obscure and difficult to understand language? Why did he force people to “read the tea leaves”? Because he knew that a single wrong word had the power to send shockwaves through the markets. “We’re going to do exactly this…” Crash goes the market.

I was thinking about this recently as I followed the increasingly interesting discussions about Ethereum’s impending Byzantium hard fork ([EIP86](https://github.com/ethereum/EIPs/issues/186) and [Reddit discussion](https://www.reddit.com/r/ethtrader/comments/6rqzmr/possible_reduction_in_block_rewards_to_3_eth_per/)).

If there is one distinguishing characteristic about this discussion, it is that there are no tea leaves. There is simply an unambiguous statement from the core developers that the per-block reward is going to be lowered from the current five ether per block to three ether per block.

Okay, maybe there is some tea, but actually we care about source code...

![](/blog/img/019-Reading-Byzantiums-Tea-Leaves-001.png)

The above two lines (and a few related lines elsewhere) implement the per-block change in the mining reward. That’s pretty unambiguous.

#### A Fully Manufactured Conversation

The following conversation between the core developers and miners was completely manufactured in my own mind:

> **Developers:** “We’re going to make a change as part of the Byzantium hard fork to lower the per-block reward from five to three…”

> **Miners:** “Wait. Why are you doing that? That will cost us a lot of money…”

> **Developers:** “You knew the code worked this way…the difficulty bomb has been in the code since the beginning…”

> **Miners:** Changing the block reward has nothing to do with diffusing the difficulty time bomb… \[the miners have a point here\].

> **Developers:** Block production on a per-day basis has been decreasing for months. After the fork, the per-day reward will bounce back. In fact, it will be higher. Three ether every 14 seconds is more than five ether every 25 seconds.\[The developers have a point here.\]

![](/blog/img/019-Reading-Byzantiums-Tea-Leaves-002.png)

> **Miners:** …but…but…that’s different…okay…there’s nothing we can do…

#### Reading the Tea Leaves

I think the developers are doing the right thing by lowering the block reward (I have a feeling the ecosystem is paying too much for security — we will explore this in [the next issue of our series](https://medium.com/series/byzantium-thoughts-6a5f6b43e86f/edit)), but their arguments are a bit disingenuous. They’ve conflated the two issues of _block reward_ and _difficulty calculation._

Let’s look at some tea leaves (so it’s math, so but let’s pretend it’s tea):

![](/blog/img/019-Reading-Byzantiums-Tea-Leaves-003.png)

Above is a description of how the difficulty is calculated at each block in the current code (pre-fork). You’ll notice there is no mention of the block reward. That’s because the block reward is in a different part of the code. [Learn more about the reasoning behind this calculation here.](https://medium.com/p/8326143c92e8/edit)

Let’s look at what’s changed between Homestead and Byzantium. The following image is a visual difference between the pre- and post-fork code. (I simplified it for clarity.)

![](/blog/img/019-Reading-Byzantiums-Tea-Leaves-004.png)

There are only two changes to the post-fork code. First, there is a small change in `section A` that makes mining uncles less likely — (see [this article](https://bitslog.wordpress.com/2016/04/28/uncle-mining-an-ethereum-consensus-protocol-flaw/) and this [EIP 100](https://github.com/ethereum/EIPs/issues/100)) and, second, there is the resetting of the difficulty time bomb at `section B`.

In order to reset the time bomb, the calculation subtracts 2,999,999 from the parent’s block number (as opposed to adding one as it did before). Assuming the Byzantium fork happens at block 4,370,000, this change acts like a time machine throwing the calculation back 529 days to block 1,370,001 (April 20, 2016 at 7:40:59 UTC). Using the same time machine to project forward, the system will return this amount of time-bombyness by March 14, 2019. Note that the remainder of the calculation remains unchanged.

#### How Will this Change Affect the Ecosystem?

One of the things Rukeyser constantly noted was that the market, knowing what it knows, discounts the future. In other words, the market builds all conceivable futures into its current price. _Buy on the rumor, sell on the news._

The change in the difficulty calculation at the Byzantium hard fork is well anticipated. Presumably, current prices already reflect it. However, (and I think Alan Greenspan might understand what I’m saying here), the change in the calculation is a step function and is definitely not obscure. I think that’s ultimately a good thing.

[In previous writings](https://medium.com/@tjayrush/ethereum-block-production-continues-to-slide-1b74a2123e3f), I’ve noted that the number of transactions per day has been increasing despite decreasing per-day block production. What will it mean to the ecosystem when there’s a sudden jump in the number of blocks per day? Will gas prices spike? Will gas prices plummet? Will the hash rate decrease or increase? Will the fiat price of ether rise or fall?

In the next installment of this series, we will explore these questions and others. In particular, we hope to explore these questions under the light of a fiat-powered spotlight—do these issues become more or less clear in the light a fait-based analysis? Stay tuned, we’ll let you know.

**Late Breaking News:** After publishing this article, I found this post [http://www.cahf.co/](http://www.cahf.co/). It seems maybe the Byzantium hard fork is a bit more contentious that I thought it might be.

We’re interested in your thoughts. Please clap for us and then post your comments below. If you enjoy our work, consider supporting us by sending a tip to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

Join our [discord channel](https://discord.gg/RAz6DJ6xkf) There’s not a lot of members just yet, but we’re working on it.

**Thomas Jay Rush** owns the software company TrueBlocks whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact him through [the website](http://trueblocks.io).
