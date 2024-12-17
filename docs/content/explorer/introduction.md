---
title: "Account Exploring"
description: "Ethereum consists of multiple different data models. TrueBlocks exposes them all."
lead: "Ethereum consists of multiple different data models. TrueBlocks exposes them all."
draft: false
url: "/explorer/introduction"
aliases:
 - "/docs/explorer/"
menu: 
  explorer:
    parent: introduction
weight: 1000
toc: true
---

<!-- markdownlint-disable MD041 MD033 -->

<img src="/data-model/img/data-model-600.png" alt="TrueBlocks is full of data that interrelates." width="600"/>

## TrueBlocks data

On its own blockchain data is an unintelligible blob of binary bytes. But, we all know there
is an amazing collection of deeply interesting information contained therein. Thousands of
people are trading, voting, expressing their preferences, making markets for valueless
items every minute.

What interests us about this data are answers to questions such as:

* What exactly is going on?
* Where is my money?
* Didn't I have some of those tokens somewhere?
* May I please just get a list of my transactions?

TrueBlocks allows you to query this type of information and more. Using the
`chifra list` option, one may list every detail of any transaction that happened
against one's own wallet address (or anyone else's for that matter).

The `--articulate` option, which is available on many commands, reanimates
or articulates the impossible-to-understand `input` and `event` data fields.
TrueBlocks even articulates trace data which reveals the deep history of
any transaction include multi-layer deep smart contract calls.

TrueBlocks works as easily with multiple related addresses at a time as it
does with individual addresses. One can view entire asset balance histories,
detailed voting histories for a DAO, or the ownership status on one's POAPs
and ENS names. We even allow exporting of Open Financial Exchange files which
is what your bank uses to export your transactions to accounting software.

## Consistent apis / interfaces

Each of the TrueBlocks endpoints and command-line tools are built on
the same core libraries (many of which are in Go). Our server (`chifra serve`)
uses the command line tools directly to complete many of its tasks, therefore
the interfaces are identical. Applications built on top of TrueBlocks have
a consistent, stable data interface.

## Open source

Furthermore, TrueBlocks is fully open
source. If you don't like something, change it and make a PR (non-commercial
use only). If you're a commercial entity, feel free to make changes, but
remember, you must either license the code separately or release your
modifications under the same open source license. Please contact us about our
licensing plans if you have a non-public, commercial use case.
