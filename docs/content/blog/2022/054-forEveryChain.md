---
title: "forEveryChain"
description: "Using the new mutli-chain features of TrueBlocks"
lead: "Come Visit with Us for a While"
date: 2022-02-15T10:45:28-04:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
images: []
draft: false
contributor: ["rush"]
tags: ["trueblocks"]
weight: 946
---

<!-- markdownlint-disable MD041 MD033 -->

Have you ever had the desire to look at every trace of every transaction of every block on every blockchain? Yes? Well, pull up a chair and visit with us for a while.

### It’s a Multi-Chain World

I’m sure many of you are aware of the coming "multi-chain" universe of blockchains. You know the one. The one where all sorts of little baby communities of people are going to be doing all sorts of little baby community-like things. Charities over here. Games down here. Ultra sophisticated blockchain finance applications up there.

If you know us at TrueBlocks at all, you know we're decentralization maximalists. That’s why it’s always interested (and perplexed) us how the above multi-chain world can possibly remain decentralized.

This week we took a stab at trying answering that question by adding a new feature to TrueBlocks that we call **multi-chain support**. This feature seems relatively simple on its face — a user only has to add `--chain <whatever>` to any of his/her chifra command lines — but it opens up a whole new world of possibilities (and headaches) — but...

```[bash]
OUR CODE STILL WORKS ON A DESKTOP      ← this should be our new logo!
```

In this article, I want to show you an example of one of the new possibilities we've seen with this new code.

I’ll start by explaining an existing example from our example code called forEveryTrace.

This example visits every block in the Ethereum Mainnet, and from there every transaction in every block, and from there every trace in every transaction in every block.

I conclude the article by talking about a new bit of code we wrote called `forEveryChain` which came into existence literally ten minutes ago and inspired this post.

### forEveryTrace

We assume a certain amount of familiarity with TrueBlocks in particular how to [install it and get it running](https://trueblocks.io/docs/install/install-core.md). We also assume you have access to your own node software. We highly recommend [Erigon](https://github.com/ledgerwatch/erigon) running locally, and while the following code will work with Infura, you may not want to do that, as it will definitely get you rate limited. Don’t say we didn’t warn you.

In our repo there’s a folder called `./examples` and in that folder, there’s an example in a sub-folder called `./forEvery`. This is the contents of the code in the folder:

<img src="https://miro.medium.com/max/1400/1*bYH13f6MYqetjD_NTVHxXg.png" width="640px">
<p>

If you’re familiar with C++ code at all, you’ll notice the `main` function at **line 34** is really simple. Initialize the library, call a single function, clean up, and finish.

While simple, that one function packs a wallop. It does exactly as it says, it visits every block in the entire blockchain it’s configured for. (By default TrueBlocks is configured to run against Ethereum Mainnet, but we’ll get back to this in a minute.)

What line 38 does is:

```[plaintext]
For every block in the current blockchain,
  call the function visitBlock (line 29) and pass it a pointer to counter.
```

What does the visitBlock function do?

```[plaintext]
For every transaction in the given block,
  call the function visitTransaction (line 24) passing in the same pointer.
```

What does the function visitTransaction do?

```[plaintext]
For every trace in the given transaction,
  call the function visitTrace (line 16) passing in the same pointer.
```

And finally, what does visitTrace do?

```[plaintext]
It keeps a count of the total number of traces in the entire blockchain and prints out each trace.
```

### An Aside on the Concept of ON THE DESKTOP

If you look carefully at this code, you’ll understand a little bit of why TrueBlocks still works on the desktop even after more than 14,000,000 Mainnet blocks.

We’re not extracting any permanent data from the chain.

While TrueBlocks provides for a binary cache that speeds up processing many times over raw node queries, the cache is optional. The cache is only employed if the end-user instructs us to use the cache.

We leave the decision of whether or not to cache to you. This is markedly different from any other solution we’re aware of and it’s why we still work on a desktop.

If you want to cache the entire blockchain because that’s your requirement — the first thing you have to do is get a really, really large hard drive — and then alter the above code to write each block, transaction, or trace to either our cache or your database.

This is the exact problem we have with solutions such as AmberData and EtherScan. They’ve chosen to extract and cache every single piece of data on the entire blockchain prior to the developer’s decision if it’s pertinent. This creates many, many Terabytes of data which means their solutions will never work on a desktop and therefore will never be truly decentralized. Enough soapbox — back to the point.

### For Every Trace on Any Chain in the Visible Universe

As I mentioned above, this week we added a multi-chain feature to TrueBlocks which means we can process queries for any EVM-based blockchain with the same equal ease as we process Mainnet.

I was inspired to write this post because I thought of the following modification to the above example.

<img src="https://miro.medium.com/max/1400/1*Qt-bK1e_eQHetmAZcsWAhA.png" width="640px">
<p>

What does this actually do?

```[plaintext]
It visits
  every trace
    in every transaction
      in every block
        on any EVM-based blockchain
  ON A FUCKING DESKTOP.
```

That's important. You can it by yourself. All you have to do is run your own nodes and run TrueBlocks.

### Support Our Work

TrueBlocks is self-funded from our own personal funds and grants from our supporters including The Etheruem Foundation (2018), Consensys (2019), Moloch DAO (2021), and most recently Filecoin/IPFS (2021).

And you!

If you like this article and wish to support please donate to our GitCoin grant https://gitcoin.co/grants/184/trueblocks. Donate during the next matching round, and we get the added benefit of the matching grants. Even small amounts have a big impact.

If you’d rather, feel free to send ETH or any other token to us directly at trueblocks.eth or 0xf503017d7baf7fbc0fff7492b751025c6a78179b.
