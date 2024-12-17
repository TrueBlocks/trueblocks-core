---
title: 'Ethereum’s Issuance: uncleReward'
lead: Second in a series about issuance
date: '2020-08-15T12:13:31.684Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 959
---

This the second in a series of two articles detailing Ethereum’s **issuance**. Read the [first part](https://medium.com/@tjayrush/ethereums-issuance-minerreward-3cad5b9a72ff), which discusses the `blockReward` calculation. Also, see [the code base](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/other/issuance) for the actual code. This article discusses the `uncleReward`.

#### Uncle Reward

Previously, we looked at the ungrammatical second sentence in Section 11.3 of Ethereum’s Yellow Paper. In [the first article](https://medium.com/@tjayrush/ethereums-issuance-minerreward-3cad5b9a72ff), we discussed the first half of that sentence concerning `blockReward`. In this article we discuss the remaining half of that sentence (shown below) which details the `uncleReward`.

![](/blog/img/041-Ethereums-Issuance-uncleReward-001.png)

It would be more accurate for the Yellow Paper to say that the “beneficiary of **_each_** ommer gets rewarded…” as there may be more than one uncle in the block, each with a different miner. We call this reward the `uncleReward`.

The calculation of the `uncleReward` is a function of the `blockNumber` in which it appears, the uncle’s own `blockNumber`, and the `baseReward` from the previous article. A block “becomes an uncle” when it shows up in the uncle list of a canonical block.

The `uncleReward` is calculated from the block’s `baseReward` reduced by `1/8` for each block the uncle is distant from the current block. If an uncle is more than six blocks old, it is not rewarded

#### Small Detour

Before we proceed, let’s take a small detour to look at the fourth sentence of Section 11.3, the only sentence we haven’t looked at yet. This sentence informs the calculation of `minerReward`.

![](/blog/img/041-Ethereums-Issuance-uncleReward-002.png)

This simplifies our concerns. It means we don’t have to keep track of previously applied rewards. The winner of a `blockReward` may also win one or more `uncleRewards` in the same block. Furthermore, a single miner may win both `uncleRewards` in the same block.

#### Back to the Main Line…

The `uncleReward` is described by the following set of three unnecessarily-complicated equations (163, 164, and 165):

![](/blog/img/041-Ethereums-Issuance-uncleReward-003.png)

We will look at each one separately. Starting with Equation 163:

![](/blog/img/041-Ethereums-Issuance-uncleReward-004.png)

where…

![](/blog/img/041-Ethereums-Issuance-uncleReward-005.png)

Translating Equation 163 into English, it says “For each uncle in the block’s uncle list (if any), the account of the uncle’s miner either (a) remains empty if it was previously empty and the `uncleReward` is zero, or (b) the account transitions to a new state (called `a-prime`) as per Equation 164”:

![](/blog/img/041-Ethereums-Issuance-uncleReward-006.png)

where…

![](/blog/img/041-Ethereums-Issuance-uncleReward-007.png)

This translates to English as, “The state of the account of the uncle’s miner after the block, `a-prime`, is the same as it was prior to the block except for the fact that balance has increased by the value _R_ (the `uncleReward`).

And finally, Equation 165 defines the `uncleReward:`

![](/blog/img/041-Ethereums-Issuance-uncleReward-008.png)

where…

![](/blog/img/041-Ethereums-Issuance-uncleReward-009.png)

Translating this final bit to English: “The `uncleReward` is the `baseReward` lessened by `1/8` for each block that the uncle is distant from the present block.”

Note that the uncle’s `blockNumber` will always be less than the block’s `blockNumber`, making the value in the innermost parentheses always negative, and thereby lowering the `uncleReward` for each block distant from the front of the chain.

#### In Code

All of the above mumbo-jumbo fits nicely into this **TrueBlocks** code:

![](/blog/img/041-Ethereums-Issuance-uncleReward-010.png)

These calculations, at least to me, are much easier when written in code than they are in the Yellow Paper .

Note that `getUncleReward` needs to be called for each uncle in the block; therefore, we must first call `getUncleCount`, and spin to query for each uncle block. Then, we can extract the uncle’s block number, and calculate `uncleReward`.

We accumulate the rewards for the uncles and return that value to the caller:

![](/blog/img/041-Ethereums-Issuance-uncleReward-011.png)

#### Rolling it Up

We’ve shown that the `blockReward` and the `uncleReward` are not that complicated, and that both are functions of `blockNumber` and `nUncles`.

Until now, we’ve shown this calculation for individual blocks. The only thing left to do is to roll through each block in the chain, accumulating the results for presentation.

We use a `C++` accumulator class we call `CReconciliation`. The name implies an additional feature that we will discuss in the [README of the code base](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/other/issuance) where we ‘reconcile’ the expected or modeled values with actual account balances directly from the chain.

There have been other proposed scripts to the Crypto Twitter uproar. Those scripts only **_model_** the behavior of the chain. They do not validate or audit the behavior. We explain this auditing process in the [README](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/other/issuance).

For now, we conclude this article with code that processes the rewards for each block.

#### forEvery…Thing…

The following code shows an excellent feature of **TrueBlocks**.

**TrueBlocks** provides many functions that start with `forEvery`, such as `forEveryBlock`, `forEveryAddressInBlock`, `forEveryTransactionInBlock`, `forEveryIndexFile`, etc.

These functions are similar in nature to Javascript’s `map` and `reduce`. One sends an arbitrary function and arbitrary data pointer to each block in the chain (or each transaction, or each address, or whatever) and allows the function to be applied to the data.

This tends to make the topmost code of a **TrueBlocks** app very minimal:

![](/blog/img/041-Ethereums-Issuance-uncleReward-012.png)

One need only define the function `calcRewards`, which we’ve done throughout these two articles.

The above code spins through each block, accumulating, auditing, and presenting the data for that block, and then reports the results of the accumulated data to the screen.

#### One Final Thought

Here, I wish to interject my own personal opinion: the Yellow Paper is stupidly over-complicated. The code that generates the Ether Supply is, in the end, trivial. For some ungodly reason, the specification wildly over-complicates it. I suppose that’s the nature of engineers with a Ph.D.

#### Support Our Work

I wish to thank Meriam Zandi for her help in this article.

Help us continue our work. Visit our GitCoin grant page here: [https://gitcoin.co/grants/184/trueblocks](https://gitcoin.co/grants/184/trueblocks), and donate today.

Or, if you’d rather not expose yourself to scrutiny, and you’d still like to donate, send ETH to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

#### Also, Read

* The Best [Crypto Trading Bot](https://medium.com/coinmonks/crypto-trading-bot-c2ffce8acb2a)
* [Crypto Copy Trading Platforms](https://medium.com/coinmonks/top-10-crypto-copy-trading-platforms-for-beginners-d0c37c7d698c)
* The Best [Crypto Tax Software](https://medium.com/coinmonks/best-crypto-tax-tool-for-my-money-72d4b430816b)
* [Best Crypto Trading Platforms](https://medium.com/coinmonks/the-best-crypto-trading-platforms-in-2020-the-definitive-guide-updated-c72f8b874555)
* Best [Crypto Lending Platforms](https://medium.com/coinmonks/top-5-crypto-lending-platforms-in-2020-that-you-need-to-know-a1b675cec3fa)
* [Best Blockchain Analysis Tools](https://bitquery.io/blog/best-blockchain-analysis-tools-and-software)
* [Crypto arbitrage](https://medium.com/coinmonks/crypto-arbitrage-guide-how-to-make-money-as-a-beginner-62bfe5c868f6) guide: How to make money as a beginner
* Best [Crypto Charting Tool](https://medium.com/coinmonks/what-are-the-best-charting-platforms-for-cryptocurrency-trading-85aade584d80)
* [Ledger vs Trezor](https://medium.com/coinmonks/ledger-vs-trezor-best-hardware-wallet-to-secure-cryptocurrency-22c7a3fd391e)
* What are the [best books to learn about Bitcoin](https://medium.com/coinmonks/what-are-the-best-books-to-learn-bitcoin-409aeb9aff4b)?
* [3Commas Review](https://medium.com/coinmonks/3commas-review-an-excellent-crypto-trading-bot-2020-1313a58bec92)
* [AAX Exchange Review](https://medium.com/coinmonks/aax-exchange-review-2021-67c5ea09330c) | Referral Code, Trading Fee, Pros and Cons
* [Deribit Review](https://medium.com/coinmonks/deribit-review-options-fees-apis-and-testnet-2ca16c4bbdb2) | Options, Fees, APIs and Testnet
* [FTX Crypto Exchange Review](https://medium.com/coinmonks/ftx-crypto-exchange-review-53664ac1198f)
* [NGRAVE ZERO review](https://medium.com/coinmonks/ngrave-zero-review-c465cf8307fc)
* [Bybit Exchange Review](https://medium.com/coinmonks/bybit-exchange-review-dbd570019b71)
* [3Commas vs Cryptohopper](https://medium.com/coinmonks/cryptohopper-vs-3commas-vs-shrimpy-a2c16095b8fe)
* The Best Bitcoin [Hardware wallet](https://medium.com/coinmonks/the-best-cryptocurrency-hardware-wallets-of-2020-e28b1c124069?source=friends_link&sk=324dd9ff8556ab578d71e7ad7658ad7c)
* Best [monero wallet](https://blog.coincodecap.com/best-monero-wallets)
* [ledger nano s vs x](https://blog.coincodecap.com/ledger-nano-s-vs-x)
* [Bitsgap vs 3Commas vs Quadency](https://blog.coincodecap.com/bitsgap-3commas-quadency)
* [Ledger Nano S vs Trezor one vs Trezor T vs Ledger Nano X](https://blog.coincodecap.com/ledger-nano-s-vs-trezor-one-ledger-nano-x-trezor-t)
* [BlockFi vs Celsius](https://medium.com/coinmonks/blockfi-vs-celsius-vs-hodlnaut-8a1cc8c26630) vs Hodlnaut
* [Bitsgap review](https://medium.com/coinmonks/bitsgap-review-a-crypto-trading-bot-that-makes-easy-money-a5d88a336df2) — A Crypto Trading Bot That Makes Easy Money
* [Quadency Review](https://medium.com/coinmonks/quadency-review-a-crypto-trading-automation-platform-3068eaa374e1)\- A Crypto Trading Bot Made For Professionals
* [PrimeXBT Review](https://medium.com/coinmonks/primexbt-review-88e0815be858) | Leverage Trading, Fee and Covesting
* [Ellipal Titan Review](https://medium.com/coinmonks/ellipal-titan-review-85e9071dd029)
* [SecuX Stone Review](https://blog.coincodecap.com/secux-stone-hardware-wallet-review)
* [BlockFi Review](https://medium.com/coinmonks/blockfi-review-53096053c097) | Earn up to 8.6% interests on your Crypto
