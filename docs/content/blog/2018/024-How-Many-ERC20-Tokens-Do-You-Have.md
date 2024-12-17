---
title: How Many ERC20 Tokens Do You Have?
lead: You may never know
date: '2018-04-20T23:11:49.781Z'
draft: false
categories:
- TrueBlocks
tags: ['trueblocks']
keywords: []
weight: 976
---

I was recently included [in a discussion](https://twitter.com/ricburton/status/986459891035525120) about why it’s so difficult to get ERC20 token balances (and other data) from the Ethereum blockchain. I thought I’d take a crack at answering the question.

I think it comes down to four words: “Incoming”, “Internal”, “Transactions”…and “Decentralization.” I’ll focus on each word individually.

#### Transaction

I’ll start with the easiest word first: “Transaction.”

![](/blog/img/024-How-Many-ERC20-Tokens-Do-You-Have-001.png)

Go into a bagel store. Buy a dozen bagels and get a receipt. Everything you need to know about your transaction is on the receipt. `Date`, `amount`, `sender` (`from`) and `recipient` (`to`). This is the same information you’ll find on every Ethereum transaction.

From your perspective (the spender), the expenditure represents outgoing money. From the perspective of the bagel shop, the transaction is incoming money and this leads to the next word on our list…

#### Incoming

If you’re the `sender` of a transaction, you know that you sent it. You physically opened your wallet, you took the money out, and you handed it to the store owner. Keeping track of spending is easy. All you have to do is remember what you did. But what about keeping track of incoming money?

I hear you say, “That’s easy?” The bagel shop simply uses a cash register. It sounds simple enough. But now we bump into the next word…

#### Internal

Every Ethereum transaction is initiated by an external account. An external account lives _outside of the blockchain_. Transactions are initiated either by a human or by a piece of software run by a human being. If you’ve used a blockchain, you’re familiar with all of this.

There has never been (will there ever be?) a transaction _initiated from inside_ the blockchain. And this makes perfect sense — a transaction requires a signature with a private key. There are no private keys inside the blockchain. If there were, because the blockchain is open, I would see it and steal all of your money in a second.

So, distinguish between “external” and “internal” transactions. “Internal” transactions are transactions that follow from “external” transactions. Saying this another way, only smart contracts can initiate an “internal” transaction.

The majority of transactions on the Ethereum blockchain consist of one external account sending money (or a message) to another external account.

![](/blog/img/024-How-Many-ERC20-Tokens-Do-You-Have-002.png)

Many other transactions (also external) are sent from an external account to a smart contract. When you think of the transactions that are being hashed into a block, you are thinking of external transactions.

![](/blog/img/024-How-Many-ERC20-Tokens-Do-You-Have-003.png)

An internal transaction is a transfer of value (or a message call) that occurs as the result of a smart contract calling or sending money into other accounts.

![](/blog/img/024-How-Many-ERC20-Tokens-Do-You-Have-004.png)

As we saw earlier, keeping track of the “send” side of an internal transaction is simple. The smart contract itself, as it generates the call, can simply emit an event. This will alert both the sender and receiver that something has happened (for example, a token transfer or token mint happened). This is how almost all ERC20 token contract work. When someone buys a token or transfers a token, in addition to recording that fact inside the smart contract’s balance map, the smart contract emits an event. I say “most” token contracts work this way because there is no hard requirement in the ERC20 standard when tokens are minted.

Here’s a quote from the spec:

> A token contract which creates new tokens SHOULD trigger a Transfer event with the from address set to 0x0 when tokens are created.

“SHOULD”, in EIP-speak, means that the contract doesn’t have to comply. And here you should be able to see one of the reasons why it’s so difficult to accurately account for an arbitrary ERC20 token.

Sometimes, when an ERC 20 token contract mints new tokens, it **does not** generate an event. This was more likely back in the day with early ERC20 token contracts, but it’s still true today.

Even when a smart contract does generate minting events, there is no standard way of doing so. Some smart contracts generate a `Transfer` event with the `from` parameter set to `0x0` as they SHOULD. Others have their own `Mint` event. Still others generate no events at all on a mint.

The fact that there is no hard standard for mint events, makes programmatically accounting for every received token impossible. This becomes especially acute in the case of airdrops, many of which create thousands (millions?) of tokens when first created. Frequently, they do not generate individual events for each minted token. It’s as if money magically appeared in your wallet or the bagel shop’s cash drawer.

Technically, an ERC20 token mint is not an “incoming internal transaction” — there’s no actual send or call into the recipient — but incoming internal transactions can have the same effect. If an event is not generated by a calling contract — and in many cases, events are not generated even when value is exchanged — the receiving account can be granted ownership of something of value without being made aware.

If the smart contracts do not generate events for value transfers, there is no way for the receiving address to know about it without watching every transaction. Watching every transaction, looking for transactions of interest, is quite difficult. (See my article, [The Trace Data Problem](https://medium.com/@tjayrush/the-trace-data-problem-d16b2e84fe40) for some context.)

If one wishes to be truly anal retentive (and for some reason, I do, I have no idea why), one must replay every trace (traces are nearly identical to internal transactions). In order to know that an unknown smart contract granted your account ownership of something, you have to be constantly vigilant.

It’s kind of ridiculous. I know!

Before this post runs into an infinite length, I’ll move on to the final word…

#### Decentralization

Getting full and accurate account-specific list of transactions (and therefore token balances) from Ethereum is hard. It’s also slow. It is so slow, in fact, that it’s nearly unusable. One simply cannot replay the entire chain every time one wishes to account for a different address. There are some solutions, but none of them good.

The two most obvious solutions are: (1) make a duplicate copy of the blockchain data including all traces and build a giant database index, or (2) keep the data on the node, do not duplicate it, and figure out some way to make scanning the chain palatable. (This is what TrueBlocks does. It’s not as fast as a database index, but it’s faster than a raw scan of the entire chain, and it’s tremendously smaller than duplicating the data and creating an index.)

If one chooses the first path, one will almost certainly be forced to use some sort of gigantic cloud computer. The index alone will be many gigabytes. And it only gets bigger over time (can you say sharding?). Worse, you have to store duplicate data, and furthermore, 99% of the data you’re storing will never be used. You’re accounting for specific accounts, not the entire chain. Most likely, your customers are only interested in the data from their accounts.

If that’s not bad enough, as soon as sharding happens the “duplicate copy of the blockchain” solution will be shit out of luck. If the data size is `X` prior to sharding, it will be `100 * X` after sharding. The only viable solution if one chooses this first path will be to centralize the data and try to sell it (either directly through an API or through advertising).

Web 2.0 is dead. Long live Web 2.0. I only have to present EtherScan as my first exhibit to support this claim.

If, instead, one chooses the second path, and tries to make accessing the node-stored data faster, so that your solution remains decentralized, you quickly realize that the code _literally wants_ to be open sourced. If you’re going to keep your code closed source, why not go all the way and centralize? That path is far easier and delivers the data far faster.

#### I’m out of Words

I’m hungry, and I’ve nearly run out of words, so I’ll end with a final thought. Developers are left with a conundrum: either decentralize and be forced to open source your code or centralize it thereby eliminating the reason we all got into this thing to being with. This, I think, is why there’s currently no viable solution to the problem. There is no appealing path forward.

I’ve now officially run out of words. If you’ve made it this far, I guess you’re happy about that. I’ll say three more words…

#### Support My Work

We’re interested in your thoughts. Please clap for us and post your comments below. Consider supporting our work. Send us a small (or large) contribution to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

**Thomas Jay Rush** owns the software company TrueBlocks whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact him through [the website](http://trueblocks.io).
