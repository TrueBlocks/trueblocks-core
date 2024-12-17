---
title: Building Your Own Ethereum Archive Node
lead: And why you may want to
date: '2020-08-13T18:21:08.933Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 961
---

This article is incomplete in the extreme.

\[Jan 20, 2021: Since writing this article, we’ve come upon a project called TurboGeth. It reduces the size of the hard drive needed for an archive node from 6TB to around 1.5TB. Significant difference. Same data. Less costs.\]

In the first part, we simply present pictures and prices for the components we used to build the two Ethereum archive nodes we run in house. This list is outdated. About eight months ago we were forced to add a second 4TB hard drive to each machine, as the Ethereum archive node we were running (Parity) was getting close to filling up our single 4TB drive.

After the pretty pictures, is the beginning of an article we started but never finished about running archive nodes and why one might wish to do that. Skip ahead if you wish.

#### Pictures of Components (and 2018 Prices) Needed to Run an Ethereum Archive Node

![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-001.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-002.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-003.jpg)

\[We recently added a second one of these in a Raid 0 configuration as the data needs of the archive node overtopped 4TB\]

![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-004.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-005.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-006.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-007.jpg)
![](/blog/img/039-Building-Your-Own-Ethereum-Archive-Node-008.jpg)

The above pictures and prices are of the components we used to build an Ubuntu machine that we use to run archive nodes. To be honest, these machines may be over built. Once they are synchronized, the easily keep up with the chain with little or no effort. We do not mine. There’s only one block every 14 seconds, so it’s not as if a lot is going on once the node is caught up.

**Building Your Own Ethereum Audit Node**

If you listen to Crypto-Twitter concerning Ethereum, you will be under the impression that running a “big, scary Ethereum archive node” is nearly impossible.

There are companies starting to play off of this impression. In fact, in the last few months, two different companies rolled out pay-to-play plans granting access to Ethereum archive nodes. One of them charges nearly $10,000 a year. Imagine that — almost $1,000 US dollars per month to access the “shared-global-ledger-that-anyone-can-read.” Sounds more like “shared-global-ledger-that-any-rich-person-can-read.”

This is simply not necessary.

At TrueBlocks we run two Ethereum archive nodes _in house_, and we’ve been doing so since early 2016. We’ve long ago depreciated the cost of the machines. The ongoing cost of running these machines is negligible. The software has been amazingly stable (knock-on-wood). They run nearly unattended.

**Outline**

To make things simple in this article, I will refer to four `modes` for running an Ethereum node: _light, full_, _tracing_, and _archive_. This translates roughly into having access to _little, some_, _most_, or _all_ of the Ethereum data.

I’ll start by explaining the difference between these modes and then proceed to describe running archive nodes. To complete the article, I’ll try to explain why I want to run archive nodes.

This article concerns the Parity Ethereum client. The concepts are similar with Geth and other Ethereum node software, but the words we use may carry slightly different meanings.

**Full, Tracing, and Archive Nodes**

There are basically four ways one can run an Ethereum node: light, full, tracing, and archive. Light nodes will be ignored in this article.

We start by describing full nodes.

_Full Node_

Running an Ethereum full node allows you to interact with the Ethereum blockchain trustlessly. In this mode, the software receives and validates each new block as it is produced. With a full node, you may view only the _recent_ state of the blockchain. A full node also allows you to broadcast transactions to the network. You cannot see deeply into the transactions (i.e. dig into the traces) or retrieve historical balances or state (i.e. dig into the archive), but you can interact effectively with the network. The time to sync a full node is much shorter than the time to sync tracing and archive nodes. You may also enable an option called “fast sync” that speeds up syncing even more.

If you only need to interact with the front of the chain — for example, you just want to interact with some dApps — a fast-sync full node is probably the best option. A full node easily keeps up with the front of the chain and takes up a couple of hundred GB on your hard drive.

While a full node allows you to transact with the network, it does not provide the deep access that TrueBlocks needs. Creating a complete, accurate, and audit-quality history of transactions for individual accounts, gaining access to historical account balances, historical token balances, and historical smart contract state is technically possible with a full node — but in practice, it’s not. Even simple queries for some historical state takes hours. For access to the rich data everyone thought we would get from the blockchain, one must run a different configuration.

_Tracing Node_

Ethereum runs Turing-complete smart contracts. These contracts execute computations on a virtual machine (the EVM).

Like Bitcoin, many of Ethereum’s transactions are simply transfers of Ether from one account to another. Unlike Bitcoin, however, many Ethereum transactions are sent to a smart contract. This means executing instructions on the virtual machine. And as these instructions execute, they sometimes generate other instructions to other smart contracts. Some people call these other instructions “internal transactions.” Following Parity, we will call them traces.

Here’s a simple example: Imagine a smart contract, that, each time it receives a payment, sends half of the payment to Alice and half to Bob. Oscar sends a transaction of two ether. During execution, the smart contract creates two messages (internal transactions/traces) — one to Alice and one to Bob. Oscar’s seemingly simple transaction generated multiple traces. You can imagine it can get way more complicated in a real life scenario such as DeFi.

A tracing node allows you to retrieve these internal transactions quickly. As we mentioned above, you could get the traces from a full node, but it would take forever as they would have to be re-created. If you want to produce an audit-quality histories, without waiting forever, you need traces.

The downside of running a tracing node is that it takes WAY longer to synchronize. There is no ‘fast-sync’ mode for a tracing node. In fact, when you start Parity with tracing enabled, you get this message: “Warning: Warp Sync is disabled because tracing is turned on.”

Furthermore, the tracing node takes up a lot more disc space (although surprisingly not that much more). The synchronization for a tracing node takes significantly longer than a full node (more than a month), utilizes more RAM and CPU, and performs astronomically more reads and writes to your hard drive. (This is why we bought an SSD drive above.)

The tracing node can produce audit-quality histories on a given Ethereum account. However, if you’re trying to build an audit-quality history of an account, there’s a problem. You need one other thing: you need to be able to verify that the process you’ve used to build the audit trail is correct.

_Archive Node_

Now that you have a tracing node and something like TrueBlocks to make the tracing node responsive to queries, why would you need an archive node?

The first thing you need to know about an archive node is that it takes up a _lot_, _lot_, _lot_ of hard drive space. 4.5 Terabytes as of the date of this writing and growing quickly.

You can run an archive node without tracing enable, but we never do. If we’re going to use four terabytes or more, why not go all the way? We run Parity with `—-tracing on` and `—-pruning archive` enabled. It takes about as long to sync as the tracing node.

When you run a full node, your node keeps only the current state — the state that exists at at blocks close to the head. All the of the history (the states that existed before that block, not to mention the interim states during transactions), are effectively thrown away. (In reality, full nodes actually temporarily cache the states of a few hundred past blocks, but this is largely an implementation detail).

A tracing node, which we discussed earlier, also throws out historical states, but keeps enough information to replay transactions. From that, historical state can be re-created in a reasonable time, but the historical state is not stored on the hard drive.

An archive node, in comparison, keeps around not only the most recent state, but the historical states of the network itself between blocks. As you might expect, this takes a ton of storage.

Both interim states and traces can all be derived on a full node — the data needed to regenerate them is there — but to do so on the fly is prohibitively slow, making it, for all intents and purposes, useless.

This is also why syncing an archive node doesn’t take much longer than syncing a tracing node. The bottleneck isn’t in what needs to be downloaded, or even in what needs to be executed by your processor. It’s primarily about the disk reads and writes that need to take place while calculating all the interim states. The big difference is that the archive node keeps the states around, while the tracing node saves only the internal transactions.

\[ THE FOLLOWING REMAINS INCOMPLETE AND PROBABLY WILL FOREVER \]

At this point, you might be asking yourself a simple question. Tracing nodes store traces, and archive nodes store interim states, but:

\[ Why not both meme\]

Good question! It turns out, traces and states are better together.

_Audit Node_

We’ve talked about regular full nodes, about tracing nodes, and finally about archive nodes. Each have their own pluses and minuses, but the ultimate node in the blockchain auditer’s toolbelt is what we aptly call an “audit node.”

Simply put, an audit node is one that stores both traces and interim states. It’s a tracing node and an archive node. If you’re going to run an archive node, you should almost certainly also keep the traces around. It doesn’t make syncing longer, and traces don’t take much extra space relative to the states.

When you operate an audit node, what you’re essentially running is a living cache. You first sync and validate all the historical transactions, keeping a bunch of data around for faster queries. Then, you continue to update this data storehouse as new blocks are mined, validating each new block, and saving the traces and states in the process.

Importantly, your audit node is yours, and you have access to this data without anyone’s permission. When you run an audit node, you’re living in the decentralized future, by trustlessly examining the blockchain’s past.

_Which Node Is Right For You?_

We’ve talked about regular full nodes, tracing nodes, archive nodes, and audit nodes. Which one is right for you? Let’s dig more into the requirements and tradeoffs that come with each node type, to help you make that decision.

First, there’s a question of whether you need to run a full node in the first place. Call us crazy, but we think virtually everyone should.

If you’re doing anything non-trivial on the blockchain, then you’re putting yourself at risk by relying on a trusted third party. This includes storing or sending any amount of ETH you’d be upset to lose. If you want to really live in the decentralized future, then running a regular full node to transact on the network is the first step.

Running a regular full node is relatively straightforward. It can be accomplished on moderate to high-end consumer hardware, including MacBooks and other laptops. What about the next step, though? Who should go from a regular full node to a tracing node, or beyond?

In short, if you need any non-trivial access to your past data, then you probably ought to consider jumping up to a tracing node. Again, we might be crazy, but we think this includes a large number of people and businesses — many more than are doing so today.

Are you actively trading ETH and other tokens? Are you borrowing or lending using Maker, Compound, or other DeFi banks? Are you paying employees or contractors in Dai? Are you trading CryptoKitties, investing in Decentraland, buying art on SuperRare, battling on Gods Unchained, funding an Aragon DAO, or working on Gitcoin grants?

In each of these cases, and many others, you should be in control of your own data. You should be capable of doing your own accounting, tracking your own statistics, and pulling your own figures for tax compliance. You should be running at least a tracing node.

A tracing node requires a slightly beefier machine and it will take much longer to sync, but it’s a huge step toward controlling your own decentralized destiny. It gives you “accounting quality” access to your data, which is good enough for many users.

Who, then, should consider jumping from a tracing node up to a full blown audit node? The answer is surprisingly simple: anyone who can afford the large solid state drive you’ll need to do so.

Syncing an audit node takes about the same time as a tracing node, and requires roughly equivalent specs from your machine in terms of processor, RAM, and disk speed. The only meaningful difference is the size of the disk you need. That extra disk space comes with a hefty price tag, but if you can afford it, we think you should spring for it.

This brings us to an important question. What does it cost to acquire, sync, and operate the various types of nodes? Until now, we’ve talked about hardware and time requirements in abstract and relative terms. Let’s now get down to brass tax, and address these concretely, based on the state of the Ethereum network, and the cost of computing hardware, in 2019.

_Hardware_

In this section, we’ll discuss in concrete terms the hardware you’ll need to run a full node. We’ll start by discussing general requirements at a high level, then dive into specific recommendations for hardware you can acquire. For the specific hardware, we’ll go from easiest to hardest in terms of time and effort.

Before we dive in, a quick note on operating systems. We recommend that you run your node on a Unix based operating system like Linux or macOS. While it’s absolutely possible to run your full node on Windows, we’ve found Unix based systems to be more stable and reliable, perhaps because of its legacy on the server.

For this reason, we’ll be covering vendors that offer machines with these operating systems out of the box. If you choose to run your node on Windows, or to buy a Windows machine and install Linux on it, you obviously have a host of other vendors to choose from, such as Dell, Lenovo, HP and countless others. The recommendations made in this section can still be useful when searching for comparable machines from those vendors.

_Specs_

A minimum viable full node carries the following base requirements:

A solid state drive (SSD) of 512 GB or greater

At least 8 GB of RAM

Intel i3 or equivalent

Your solid state drive needs to be fast — preferably it would be connected via NVMe (Non-Volatile Memory Express), but this is not strictly required. While a full node does not currently require 512 GB, it’s best to have at least that much space for future proofing. Finally, a reasonably fast multi-core processor is needed. Most modern processors would fit the bill.

When jumping up to a tracing node, we recommend the following specs:

A solid state drive (SSD) of 1 TB or greater

At least 16 GB of RAM

Intel i5 or equivalent

Again, the space requirements here allow some breathing room. In reality, a tracing node could run today with 512 GB or even less.

Finally, to jump up to a full blown audit node, we recommend the following specs:

A solid state drive (SSD) of 8 TB

At least 32 GB of RAM

Intel i7 or equivalent

Once more, these specs are not strictly required for an audit node, but they help future proof your investment. Today, you could probably get away with the same specs of a tracing node, plus a 4 TB SSD, but a year down the road, you might need to upgrade.

_DAppNode_

DAppNode is a company that offers the absolute easiest way to get a node running: they sell you one that’s already set up. DAppNode’s machine’s come with Linux and your node software already installed and configured. They’re also (mostly) synced as well.

DAppNode also layers some custom software to make configuring and updating your node easier. If you’re looking for a “plug and play” solution, DAppNode is as close as we have.

\[ DAppNode Product Page\]

As you can see, DAppNode currently offers three models: Basic, Advanced, and Archive.

The Basic model ($640 USD) can easily run a regular full node, and might barely manage to run a tracing node.

Their Advanced model ($885 USD) is a better option for tracing node, and we think it’s worth the price for the very large gain in functionality that running a tracing node gives you.

Finally, their Archive model ($1,725 USD) is technically capable of running an Audit node today, however the 4TB harddrive makes it hard to recommend. The fully cached state will likely surpass that soon. We hope DAppNode will consider offering an upgraded model soon.

Overall, DAppNode is a great option. They give you most of what you want out of the box, and as you’ll see, their prices are actually quite competitive with generic hardware vendors.

_System76_

System76 is a beloved vendor of laptop and desktop computers that come pre-installed with Ubuntu Linux. Their machines are affordable and remove the stress usually associated with having to configure Linux yourself.

In the chart below, we summarize configurations for both desktop and laptop machines that can serve your needs for running each of the various node types.

System76 Desktop

\[ TODO: turn this into a table \]

Full Node — $558

Meerkat

Intel i3–8109U

8 GB DDR4 RAM

500 GB NVMe

Tracing Node — $896

Meerkat Base

Intel i5–8259U

16 GB DDR4 RAM

1 TB NVMe

Archive Node — $3,346

Thelio Base

AMD 3rd Gen Ryzen 7 3800X

32 GB DDR4 RAM

250 GB NVMe + 12 TB Secondary SSD

System 76 Laptop

Full Node — $1,024

Galaga Base

Intel I5–8265U

8 GB DDR4 RAM

500 GB NVMe

Tracing Node — $1,263

Galaga Base

Intel I5–8265U

16 GB DDR4 RAM

1 TB NVMe

Audit Node — $4,162

Serval WS Base

Intel I7–9700K

32 GB DDR4 RAM @ 2400 MHz

240 GB SSD + 2x4 TB Secondary SSD’s

As you’d expect, there’s a premium to be paid for laptops. You’ll have to decide if portability is high enough priority to warrant the extra price. Overall, though, System76 provides a great option for acquiring an affordable Linux computer without the headache of installing Linux.

Apple

Apple is a company that needs no introduction. While they’ve come to dominate the tech landscape through the iPhone, they’ve been building premium computers for a rabid fanbase for more than 40 years.

Macs are fine options for running your Ethereum node, but they come with a big caveat: price. Apple makes machines with a focus on design and aesthetics, and the people who want to run macOS are willing to pay a pretty penny for them. For this reason, we really only recommend using a Mac for a full node if you already have one, if you want to buy one for regular use anyway, or if price is no object.

The table below summarizes the minimum configurations we recommend for Apple hardware, highlighting the model and the specific upgrades needed. The list is by no means intended to be exhaustive.

\[ TODO: Turn this into a table \]

Apple Desktop

Full Node — $1,199

Base Mac Mini

512 GB SSD Storage

Tracing Node — $1,899

Base Mac Mini

Intel Core i7, 3.2GHz 6‑core 8th‑gen

16 GB DDR4 RAM

1TB SSD Storage

Audit Node — N/A

Apple Laptop

Full Node — $1,699

Base MacBook Pro 13’

512 GB SSD Storage

Tracing Node — $2,099

Base MacBook Pro 13’

1 TB SSD Storage

16 GB DDR4 RAM

Audit Node — N/A

One thing you might notice is that there’s no configuration for an audit node. Currently, Apple does not sell a machine that fits the bill. They will be releasing a new Mac Pro in Fall 2019, but we estimate the price for a suitable machine would be upwards of $8,000.

Build Your Own

It even worse than that. Even if you do spend extra money to get yourself suitable hardware, and you’ve spent weeks syncing your node, getting usable data from the chain is horribly cumbersome. This is where our project, TrueBlocks, comes in. TrueBlocks creates “the missing Ethereum index” directly from the blockchain data. This index unlocks the chain’s full potential. We’ll talk more about this later.
