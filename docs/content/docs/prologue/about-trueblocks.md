---
title: "About TrueBlocks"
description: "How does TrueBlocks address the problem"
lead: "How does TrueBlocks address the problem."
date: 2021-10-04T10:03:01-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu: 
  docs:
    parent: prologue
weight: 0201
toc: true
---

## TrueBlocks provides a solution

We think there are at least three fundamental problems with the current Ethereum ecosystem:

1. It's difficult to run your own Ethereum node (not really, see below).
2. Even if one runs a node, getting useful data is nearly impossible.
3. All current solutions end up with a winner-take-all central database just like Google.

We discuss each of these problems below.

### Ethereum nodes are difficult to run

Vitalik once called the Ethereum client software a "Big, Hairy node." No wonder people think they are hard to run.

But, with the recent arrival of [dAppNode](https://dappnode.io/) and [Avado](https://ava.do/) things
have gotten much easier. Each of these products allows you to buy a machine, plug it in the wall,
and your soon you're running your own node.

You should to this now. Buy a dAppNode, plug it in, and enjoy the benefits of running your own node. Why wait?

### Useful data is hard to get

It is our strong belief that the Ethereum RPC is broken. It's missing something. An index by address.

The goal of TrueBlocks is to provide exactly that missing piece. We call our solution the [Unchained Index](/docs/install/build-unchained-index/).

The Unchained Index is created locally on your machine -- on the dAppNode. As a direct result of being local, the index has a number of amazing qualities:

1. **It fixes the broken node:** Without an index, one cannot query the node for a "list of everything that ever happened to an address." With one, which is what the Unchained Index provides, one can make such a query. In effect this turns the node from a "really shitting database," into a perfectly fine database.
2. **No rate limiting:** Because both your node and the Unchained Index are local and not shared with other users, there is no need for rate limiting. Your can query the local node 100s of times faster than you can an API. This has massive implications for the types of applications you can build.
3. **Perfectly private:** All queries happen against your local node - you don't expose your IP address to anyone, nor do you expose any information about which addresses you're interested in.
4. **Faster queries leads to a better index::** Because we can query so much faster, we can dig MUCH deeper into the details of every block. This means our index contains more information about where an address appears on the chain. In fact, in tests against two popular Ethereum APIs, we consistently find 20-30% more appearances.
5. **More appearances means more accurate results:** because our locally created index goes to a deeper level of detail, we can deliver to end user applications much more informative data. For example, we can fully reconcile transactions off-chain. Something not even the most expensive web-based services can do.
6. **Unique user experience:** Because TrueBlocks is focused on the "account", the user experience is cohesive -- everything about an account is brought together in single interface. No more visiting a different website for each different dApp you interact with.

TrueBlocks ships with large suite of command line tools that allow you to make exactly the queries you want to make. One of the
command line options is an API server which allows you to build blindly-fast desktop applications. We've built an example of
such an application with our [Account Explorer](/docs/install/install-explorer).

### Avoiding winner-take-all

In it's best incarnation, TrueBlocks runs locally against your own locally running node. If you build the thing yourself, no-one can take it away from you. After the initial synchronization of your node (Erigon, for example) which may take a few weeks, you can run one of TrueBlocks' command line tools called [chifra scrape](/chifra/admin/#chifra-scrape). This process takes a few days.

To ease the burden of having to create the Unchained Index yourself, you may download it from IPFS using [chifra init](/chifra/admin/#chifra-init). Putting this data on IPFS has the very interesting consequence of sharing it. This makes it impossible for us (or anyone) to become the "holder of all data" which eventually leads to winner-take-all like Google. We want to purposefully avoid that.

This whole process is [better explained here](/docs/install/build-unchained-index/).

## Unchained Index provides huge advantages

Besides the philosophical benefits, TrueBlocks is straight up more performant.

- Querying straight from your hard drive is _100s of times faster than an API_.
- A local binary cache makes subsequent queries _nearly instantaneous_.
- Avoiding a full extraction of the data from the node results in much lower hardware requirements: from _terabytes to gigabytes_.
- Bloom filters further reduce storage overhead and greatly lessen the search time for particular addresses.
- Articulation resolves the transactional detail at the byte level, _turning byte streams into human-readable data_.
- The data can be exported into many formats: _JSON, CSV, TSV, OFX, etc._

TrueBlocks performs so well because the design data first. We are lifelong hackers, and we agree with [Linus Torvalds when he
said](https://lwn.net/Articles/193245/):

> In fact, I'm a huge proponent of designing your code around the data, rather than the other way around.

## More words

We've got more words for you. [This blog post covers these topics in much more detail](/blog/a-long-winded-explanation-of-trueblocks/).
