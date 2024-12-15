---
title: Announcing TrueBlocks®
lead: Finally...
date: '2017-10-29T18:07:27.150Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 980
---

We are pleased to announce the release today of the first of three white papers describing TrueBlocks: [Faster, Richer, Fully Customizable Data from Programmable Blockchains](http://trueblocks.io/wp-content/uploads/2017/10/2017-Rush-Faster-Richer-Fully-Customizable-Data-from-Programmable-Blockchains.pdf). Additionally, we are announcing the release of an updated and improved website ([http://trueblocks.io](http://trueblocks.io)) as well as fourteen open-source command line tools.

![](/blog/img/020-Announcing-TrueBlocks-001.png)

#### What is TrueBlocks

TrueBlocks is a collection of software libraries, open source command-line tools, and applications intended to improve the ease and speed with which one may access data from the Ethereum blockchain. Given fast, easily accessible data, new use cases and applications such as smart contract monitoring, per-block accounting, and integration with business intelligence tools such as Tableau become possible.

#### Open Source Command-Line Tools

As part of our commitment to the Ethereum community, we offer fourteen open source command-line tools including:

* **getBalance** and **getTokenBal**: which report on ether and token balances respectively for one or more Ethereum address. Reporting for any of three denominations (ether, wei, or US dollar fiat) is a simple command line option.
* **getBlock**, **getTrans**, **getReceipt**, **getTrace**, **getLogs**, **getBloom**: each of which reports on a particular data structure given one or more block hashes, transaction hashes, block numbers/transaction id pairs, or any combination.
* **ethName**, **whenBlock**, **isContract**, and **whereBlock**: tools useful in retrieving information about Ethereum account names, when certain blocks occurred, whether or not certain blocks are cached or reside on the node, and whether or not an Ethereum address is a smart contract.

#### Examples of Using the Command Line Tools

Having been born and raised in a Linux only shop, each of these tools works and plays well with the Linux philosophy of feeding the output of one command into the next, so you can do something like:

ethName omisigo -a

to get the address of the Omisigo smart contract. You can also do:

getAccounts

to list all of the accounts held on your local wallet. Stringing these two commands together and feeding the output into **getTokenBal** yields:

getTokenBal \`ethName omisigo -a\` \`getAccounts\`

which reports on the number of Omisigo tokens one holds. You can furthermore use a tool such as `whenBlock` to deliver monthly, weekly, or daily reports on your holding. Each tool accepts a`--dollar` option which converts ether into dollar spot prices.

![](/blog/img/020-Announcing-TrueBlocks-002.png)

To be honest, the combinations are nearly endless. Check out [our documentation](http://trueblocks.io/docs/) for more information.

#### Where to from Here?

We have many plans for the software including some unique takes on how to deal with the Ethereum data. We’ll be presenting some of our ideas over the coming month with the release of two more nearly complete white papers. We welcome any and all participation and look forward to your interest in our work. We will be at DevCon 3. [Contact us](http://trueblocks.io/#contact) through our website. We’d love to talk to you.

We’re interested in your thoughts. Please clap for us and post your comments below. Please consider supporting our work by sending a small tip to 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

**Thomas Jay Rush** owns the software company TrueBlocks whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact him through [the website](http://trueblocks.io).
