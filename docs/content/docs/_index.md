---
title : "Introduction"
description: "TrueBlocks Docs"
lead: ""
date: 2020-10-06T08:48:23+00:00
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
weight: 0101
toc: true
---

<!-- markdownlint-disable MD041 MD033 -->

Are blockchains really meeting their promise?

## Problem Statement

If you're like us, you fell in love with blockchains because of the promise they provide: world-wide, private, and permissionless
access to the most accurate and agreed-upon data ever produced. Near perfect data.

But in reality, it's as if we're just lost at sea. Awash in bytes.

<img src="/docs/img/steam-boat-off-a-harbor.jpg" alt="Who's stearing this boat?" width="500"/>

## World-wide

Blockchains are supposed to be world-wide. Are they? Yes, they are.

## Permissionless

Blockchains are supposed to be permissionless. Are they? Not really.

## Accurate

Blockchains are supposed to be accurate. Are they? Well...on-chain they are, but
off-chain, not even close. Ask any crypto-accountant.

## Problem statement

It is true that anyone may send a transaction to a blockchain. All one needs is a wallet and access to an RPC...
wait...access to an RPC...is that permissionless? Unless you're running your own node, no, it's not.

It's worse than that. Not only must you ask permission to send a transaction, you must
ask permission to see the results. "Please Mr. Massive Blockchain Explorer in the Sky...
please may I see my transactions? Oh, and by the way, you're not watching me are you?"

Worse even yet, have you ever actually tried to deeply understand what the blockchain explorer
is showing you? And why, after paying hundreds of dollars, does you crypto tax report have
massive, obvious errors?

TrueBlocks can't solve the first problem -- eventually you're going to have to provide your
own access if you want the true benefits of decentralization. Luckily there are emerging
solutions such as [dAppNode](https://dappnode.io/) and [Avado](https://ava.do/).

TrueBlocks can, however, solve the other two problems -- accurate, private access to your
own transactions.

---
Like Infura, anyone may visit EtherScan. That is true. Both of those systems are kind enough to
give us (give us!) whatever we ask for. But how long do you think it will take before they realize
(hint: they've already have) how valuable their position in the ecosystem is? Do you think they're
not paying attention to what you're doing? Do you think they're not connecting your Ethereum address
to your IP address? I would be if I were them.

Websites and APIs are inherently the wrong solution for decentralized data for a number of reasons:

1. They can see who you are because your browser tells them your IP address (not private!).
2. Because their resources are shared among many users, they must protect their system by rate limiting you (not permissionless!)
   1. They use your IP address to rate limit you - that is, they know who you are each time you arrive
3. Every time you visit EtherScan or use Infura, you do it with the same Ethereum address
4. They know who you are -- they know your address -- they know your spending habits -- that's terrible for you

Websites and APIs are terrible for you. If you think your privacy is being invaded today, wait until you start
telling them, with near perfect data, exactly what you spend your money on.

We all see this coming. No-one seems to want to do anything about it. In fact, we see an increasingly competitive
environment to provide you ingress and egress to the chain.

## Solution

[dAppNode](https://dappnode.io/), [Erigon](https://github.com/ledgerwatch/erigon) and TrueBlocks.
