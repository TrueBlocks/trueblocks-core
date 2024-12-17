---
title: Indexing Addresses on the Ethereum Blockchain
lead: Appearances are the key
date: '2019-07-09T02:30:51.814Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 969
---

Given a blockchain, our goal was to create a list of every appearance of every address in any block on that chain. We define an “appearance” as either the use of the address in one of the common ‘address’ fields of a transaction (such as `to`, `from`, or `contractAddress`) or its use as data in one of the data fields in a transaction. We do not check numeric fields such as `value` or `gasUsed`.

An appearance consists of an `< address / blockNum / tx_id >` tuple.

Given this list of appearances, we build an index sorted by address. This makes subsequent queries for all transactions on a given address easy and fast.

With a list of `transactions of interest`, we can then go back and query the blockchain for the details needed to do a full audit-quality, 100% decentralized, permissionless accounting, including “data accounting,” which means exposing the changes and current values of any of the smart contract’s state.

Because the ideas we’ve been working with are difficult to explain, especially to someone who is not familiar with the data structures used by Ethereum (as we suspect is the case for most of our readers), we try to explain what we’ve done through the following series of examples.

One thing to note. Every block is independent of every other block when it comes to extracting addresses in that block. Because of this, we are able to parallelize the work. We recently did that using golang in the latest version of our code called ‘blaze.’ We’re going to be in Berlin this summer. Hit us up if you’d like to discuss our work.

Examples follow…

![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-001.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-002.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-003.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-004.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-005.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-006.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-007.png)

Each of the above examples is relatively easy. The addresses “appear” in obvious places in the transaction such as `from`, `to`, or as the event sender data in a log. There are more difficult cases. For example, a simple token transfer or a multi-sig wallet interaction.

![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-008.png)
![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-009.png)

The above examples, don’t explain everything. There are many, many edge cases and special cases. For example, if a smart contract reverts during its construction, the contract’s address is reported in the transaction receipt’s `contractAddress` field, but it does not appear anywhere in the transaction’s traces. In general, one can find every address by searching Parity’s traces and/or the event logs, but not in this case. There are other odd examples like this.

Because our process uses Parity’s traces, we must run `parity –tracing on`. This is made much easier with a product such as [DAppNode](https://medium.com/u/8d628dbdf3c2) which will one day run our software (we hope).

#### Caveats

There are a few unfortunate facts about the way the Ethereum blockchain stores data. The fact that we must remove `baddresses` and `precompiles`, and the fact that we need to create _fake_ transactions in order to fully account for every address is fragile but necessary. It’s obvious to us that the designers of the Ethereum blockchain did not think through every edge case as relates to accounting for the chain — especially if one wishes to do this in a fully decentralized way. We suggest the designers of the next generation of blockchain clients pay more attention to the needs of those who wish to account for the system.

A careful reader will note that we only extract **appearances** from the blockchain. We do not extract any other data. This is an engineering choice we made because it was important that our system is able to run on commercial-grade hardware — by that we mean laptops. TrueBlocks works on a laptop! To us, this means “fully decentralized.” We gave up certain things in our design, such as broad statistical knowledge across the whole chain, but we wanted to build permissionless accounting all the out at the very edge. We’ve accomplished that.

As we extract the appearances, we periodically ‘consolidate’ the block-by-block data into a more easily searchable index. Unfortunately, it’s not as simple as shoving the appearances into an old-fashioned database. There’s nothing that stops us from doing that, but (as you might expect if you know anything about decentralized computing), it’s way more complicated than that. We want to make the index we’ve created easily sharable, uncapturable, undeniable, verifiable, and fully decentralized.

We’ll write about that in our next article.

#### Conclusion

We fell in love with the idea of `permissionless accounting` about four years ago when we first heard [Joe Lubin](https://medium.com/u/20ec8468cfbe) discuss the idea of triple-entry accounting and radical transparency. We’ve been working on that idea ever since, and recently — not the least due to our Ethereum Foundation grant — we’ve been making great progress. Just a quick preview of an application we’re building on the index and then we’re done. We didn’t ask if it was okay!

![](/blog/img/031-Indexing-Addresses-on-the-Ethereum-Blockchain-010.png)

#### Support My Work

I’m interested in your thoughts. Please clap for us and post your comments below. Consider supporting our work. Send us a small (or large) contribution to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

Thomas Jay Rush owns the software company TrueBlocks whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact him through the website.
