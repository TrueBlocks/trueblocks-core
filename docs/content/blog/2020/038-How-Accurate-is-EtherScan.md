---
title: How Accurate is EtherScan?
lead: WTF?
date: '2020-06-12T03:59:52.871Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 962
---

Why build an 18-decimal place accurate ledger if it doesn’t balance?

\[[Accompanying Video](https://www.youtube.com/watch?v=IFbH010A-QA)\]

I had a call this morning with a cryptocurrency accountant. He’s a wonderful fellow. One of those people who can happily wade through thousands of rows of a spreadsheet trying to get the digits to behave themselves. He’s a man after my own heart.

This accountant — call him Mr. Green — makes a good living helping people do their crypto-taxes. He’s busier than ever. He tells me, and here I quote, “Nothing ever balances.”

How is this possible? Isn’t Ethereum supposed to be an 18-decimal-place accurate ledger? Doesn’t everyone using Ethereum have the same data?

The answer, of course, is that Ethereum always does balance.

Why then is Mr. Green having trouble? It’s because he’s not using the Ethereum data directly. He’s using OPD (other people’s data). Very few of us are using data straight from the chain. We almost all of us get our Ethereum data from an API or a website. In the Ethereum ecosystem, this means either Infura or EtherScan.

I, like Mr. Green, want those annoying little digits to behave themselves. I’m becoming increasingly concerned, especially as smart contracts become more and more complicated, about the fact that “Nothing ever balances.”

#### Think before Tweeting…

This morning, I tweeted that EtherScan misses nearly 15% of what I call “appearances.”

Of course, this was met with crypto-twitter skepticism, so I‘m writing this article as a way of showing my readers what I meant by that comment.

#### What is an Appearance?

I’ve written about appearances [here](https://medium.com/@tjayrush/indexing-addresses-on-the-ethereum-blockchain-5c0806161eb9). I’ll quote from that article:

> We define an “appearance” as either: (a) the use of the address in one of the common “address” fields of a transaction (such as `_to_`, `_from_`, `_eventEmmitter_`, or `_contractAddress_`) or (b) its use as data in the `_input_` or `_eventData_` fields of the transaction, or (c) its subsequent use in any smart contract invocation.

In other words, given the byte string representing an address, an address’s appearance list is a list of every transaction where that byte string shows up. Sounds simple.

It’s not!

To try to solve this, TrueBlocks builds an index of every appearance we can find on the chain. To test our code, we use EtherScan’s APIs. You’ll understand how surprised we were this week when we discovered that TrueBlocks consistently finds more appearances than EtherScan.

> _Want to read this story later? Save it in_ [_Journal_](https://usejournal.com/?utm_source=medium.com&utm_medium=blog&utm_campaign=noteworthy&utm_content=eid7)_._

#### EtherScan APIs

EtherScan provides [five account-related APIs](https://etherscan.io/apis#accounts) which we use in our testing:

* [Get a list of ‘Normal’ Transactions By Address](http://api.etherscan.io/api?module=account&action=txlist&address=0xddbd2b932c763ba5b1b7ae3b362eac3e8d40121a&startblock=0&endblock=99999999&sort=asc&apikey=YourApiKeyToken)
* [Get a list of ‘Internal’ Transactions by Address](http://api.etherscan.io/api?module=account&action=txlistinternal&address=0x2c1ba59d6f58433fb1eaee7d20b26ed83bda51a3&startblock=0&endblock=2702578&sort=asc&apikey=YourApiKeyToken)
* [Get a list of “ERC20 — Token Transfer Events” by Address](http://api.etherscan.io/api?module=account&action=tokentx&address=0x4e83362442b8d1bec281594cea3050c8eb01311c&startblock=0&endblock=999999999&sort=asc&apikey=YourApiKeyToken)
* [Get a list of “ERC721 — Token Transfer Events” by Address](http://api.etherscan.io/api?module=account&action=tokennfttx&address=0x6975be450864c02b4613023c2152ee0743572325&startblock=0&endblock=999999999&sort=asc&apikey=YourApiKeyToken)
* [Get \[a\] list of Blocks Mined by Address](https://api.etherscan.io/api?module=account&action=getminedblocks&address=0x9dd134d14d1e65f84b706d6f205cd5b1cd03a46b&blocktype=blocks&apikey=YourApiKeyToken)

Quite some time ago we built a tool called `ethslurp` that makes use of these endpoints:

![](/blog/img/038-How-Accurate-is-EtherScan-001.png)

Given an address, `ethslurp` returns all the transactions where EtherScan says that address appears.

This week we built a shell script, `fromES`, that successively calls each of these five endpoints and assembles the results into a single file and then compares those results against TrueBlocks.

We ran tests against 99 randomly chosen addresses. For example, the command:

fromES 0x91c5fa6872f3a93b999843eaf06eb34a18a69a12\`

produces these results:

![](/blog/img/038-How-Accurate-is-EtherScan-002.png)

Notice that the five EtherScan endpoints deliver 31, 0, 26, 2, and 0 records respectively. Summed, in the line labeled ‘all’, this totals 59 records.

At first blush, we were concerned that this was more records than TrueBlocks returns (40). What we discovered was that EtherScan’s five endpoints include duplicate records. We altered our shell script to remove the duplicates producing the line ‘sorted uniq’.

Looked at it this way, TrueBlocks finds one extra record not found by EtherScan. It turns out to be transaction #`8156524.14` (that is, the 14th transaction of block 8,156,524).

TrueBlocks provides a few other tools that allow us to see the details of any Etherum transaction. The`getTrace` tool:

getTrace 8156524.14

returns

![](/blog/img/038-How-Accurate-is-EtherScan-003.png)

which I admit is a bit crazy looking, but it does contain the byte string for the given address (colored pink). It turns out that this transaction ended in a `revert`. This is probably why EtherScan doesn’t find it.

Etherscan obviously finds all transactions of type (a) mentioned above. It finds most type (b) transactions as well (EtherScan calls these `internal transactions`). If this had not reverted, it would probably have been an internal transaction because (the address is used as data as opposed to being one of the `address-related` fields).

This happens frequently on EtherScan. Of the 99 addresses we tested, TrueBlocks found more transactions than EtherScan did for 85 addresses. For not a single address did EtherScan identify a transaction that TrueBlocks did not find. 85–0. I call that significant.

#### But Is It Relevant?

I once spoke a different crypto accountant (Mr. Greenish-Blue?) and after listening to me talk about this issue, he asked, “Yeah, but is it relevant?” My reaction was “Of course, it’s relevant.” (I could explain why this is true, but if it isn’t self-evident to you already, I wouldn’t be able to convince you anyway.)

Why build an 18-decimal place ledger that the entire world comes to agreement on and spends way more on than any previous computing system if we’re going to throw away data? Explain that to me, please.

#### Analyzing the Differences

Since we first published this article, a few people have asked us to further analyze the differences. To be honest, we really just don’t have the time. Perhaps someone else will pick this up. We published the raw data to this IPFS hash: QmYccKCCoYat8AsgMxVfFjcb9NG8cnFfikkrweRyjwo7rQ.

As a quick summary, the missing transactions appear to be of at least six types:

* `Input` data of both errored and completed transactions
* Log `topics` of both errored and completed transactions
* Log `data` of of both errored and completed transactions
* Output data of deeply embedded traces
* Uncle mining rewards

An example of the first type is this transaction:

[https://etherscan.io/tx/ 0xa4a96ca16373239fd679711b05bcbdc138bc40a5bb2a085799c23bbaf5fd2a3a](https://etherscan.io/tx/0xa4a96ca16373239fd679711b05bcbdc138bc40a5bb2a085799c23bbaf5fd2a3a)

which does not appear in the internal transaction list of the address:

`0x28f4a17f8a99ab90c1a401b85d694b2c0ea40c4b`

however, that address is clearly in the input data of the transaction.

#### Future Work

We’re going to dig deeper to see if we can’t figure out what some of the other missing transactions look like. We already know that EtherScan does not report on Uncle miner rewards from its Miner endpoint. We’ll write again as we discover more.

Help us continue our work. Visit our GitCoin grant page here: [https://gitcoin.co/grants/184/trueblocks](https://gitcoin.co/grants/184/trueblocks), and donate today.

Or, if you’d rather not expose yourself to scrutiny, and you’d still like to donate, send ETH to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

📝 Save this story in [Journal](https://usejournal.com/?utm_source=medium.com&utm_medium=noteworthy_blog&utm_campaign=tech&utm_content=guest_post_read_later_text).

👩‍💻 Wake up every Sunday morning to the week’s most noteworthy stories in Tech waiting in your inbox. [Read the Noteworthy in Tech newsletter](https://usejournal.com/newsletter/noteworthy-in-tech/?utm_source=medium.com&utm_medium=noteworthy_blog&utm_campaign=tech&utm_content=guest_post_text).
