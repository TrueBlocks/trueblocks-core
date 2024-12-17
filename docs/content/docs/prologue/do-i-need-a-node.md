---
title : "Do I need a node?"
description: "A node is not required, but that's the only way use Trueblocks as it is intended."
lead: "If you want to use TrueBlocks to its full potential, then, yes, you need
a node. But if you don't have a node, we use the IPFS to provide some workarounds."
date: 2020-10-06T08:48:45+00:00
draft: false
aliases: "/docs/prologue/do-i-need-a-node"
images: []
menu:
  docs:
    parent:
      prologue
weight: 0401
toc: true
---

And if you are running your own node, TrueBlocks works best with Erigon.

----

## Why having a node is the best option

Some benefits of TrueBlocks can be realized only when a user has their own node.
Having a node lets users:

* Limit queries to the node on their machine
* Use the node to build a local index
* Query data directly from the top of the chain

Setting up a node is not as hard as many people think, but it does require two TBs of storage
and some good hardware.

If you want to build your own index, plan for a few days and run `chifra scrape`.
Before doing so, you probably want to read the relevant section in the
["How Can I Get the Index"](/docs/install/build-unchained-index) article,
and [the `scrape` command documentation](/chifra/admin#chifra-scrape).

Fortunately, TrueBlocks also publishes the index on the IPFS,
which lets you access it locally without running a node.

## Without a node, you can download the index from the IPFS

Downloading the IPFS manifest is a good option for many users:

* The initial time to get started is much faster
* The index occupies only the space you need, (between 1 and 80GB)
* You can verify its correctness with the TrueBlocks source code
* You can own the index on your local machine (though you still need to query remote RPCs for transaction data).

There are two commands to download the index, `chifra init` and `chifra pins`.
The pros and cons of each approach are outlined in the
["How Can I Get the Index?"](/docs/install/build-unchained-index) article.

### Drawbacks of using the IPFS manifest

The drawbacks of using the IPFS manifest are the opposite of the benefits
of building an index from your own node:

* You still must query transaction data from remote RPCs
* You must trust the data TrueBlocks publishes (though you can verify the build)
* You can only query up to the last time TrueBlocks published the manifest
