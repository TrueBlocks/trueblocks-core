---
title: Knowing the Future and Proving You Know It
lead: How blockchains can help
date: '2016-07-13T03:44:29.907Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 994
---

I’m building a piece of software called [EthSlurp](http://ethslurp.com). It’s a blockchain data scraper. It runs from the command line and takes various parameters such as an Ethereum contract address, an output format, a date and/or block range, and various other options.

The most salient feature of EthSlurp is its ability to convert a transaction’s **input** data (which is usually seen as unreadable hex) back into human readable text.

Every ethereum transaction carries with it an arbitrary amount of data in a field called **input**. The **input** field is one of the nuggets of true genius behind what Vitalik Buterin did — Bitcoin also carries input data in each transaction, but only 80 bytes worth. With Ethereum, the **input** data can be as long as needed. Ethereum sends function calls to smart contracts in the **input** field. In fact, the byte code of the smart contract itself is sent in the **input** field of the transaction that deploys the contract.

#### Converting Input Hex Back into English

When a smart contract is compiled into byte code, an ABI ([Application Binary Interface](https://en.wikipedia.org/wiki/Application_binary_interface)) is created. Using that ABI, which contains a list of all the function calls with each of their parameters, EthSlurp is able to convert the **input** back into a human readable form.

For example, this **input** from a typical transaction on The DAO smart contract:

> 0xc9d27afe00000000000000000000000000000000000000000000000000000000000000440000000000000000000000000000000000000000000000000000000000000001

represents this function call:

> vote(68,“Yea”)

(A “yes” vote on proposal number 68.)

While this **input**:

> 0x23b872dd000000000000000000000000bf6cc4a882ace8e3a19bb8f5372280b33b2f6c9e000000000000000000000000fbb1b73c4f0bda4f67dca266ce6ef42f520fbb980000000000000000000000000000000000000000000000056bc75e2d63100000

represents the function call:

> transferFrom(“0xbf6cc4a882ace8e3a19bb8f5372280b33b2f6c9e”,“0xfbb1b73c4f0bda4f67dca266ce6ef42f520fbb98”,“-1”)

That is, send all the ether from the first account to the second. (-1 means send all of the ether.)

I’ve been using EthSlurp to scrape The DAO every day since it first went live back in late April. I’ve stored that data, representing more than 135,000 transactions, here: [http://daodeepdive.com/data.html](http://daodeepdive.com/data.html). I’ve separated the transactions in various ways including day-by-day and function-by-function.

#### So How Did I Predict the Future?

In addition to The DAO smart contract, I also scrape other smart contracts, among them the DarkDAO and The DOA Curator’s MultiSig contract.

Yesterday, around noon EST, I noticed a bunch of transactions that I hadn’t seen before. For the first time since a few days after the theft, there were transactions on the DarkDAO, and because EthSlurp allows me see the function calls, I noticed that the new transaction represented a ‘changeAllowedRecipients’ call. So I scraped that transaction:

> [0x9257c2e0a11de7b7427d4607f5908d6448278070bb73500139387930826fedc0](https://etherscan.io/tx/0x9257c2e0a11de7b7427d4607f5908d6448278070bb73500139387930826fedc0)

whose data input field translates to this:

> changeAllowedRecipients(“da4a4626d3e16e094de3225a751aab7128e96526”,1)

I then scraped that address, and recognized it as The DAO Curator’s Multisig contract. So now I knew that somehow the DAO Curator’s Multisig was put on the Dark Dao’s whitelist.

#### Oh My God! I’m the Only Person in the World Who Knows This

When I first saw this, I was pretty sure I was the only person on earth who knew about this transaction other than the White Hat group and perhaps the Dark DAO Dick (as I call the attacker). Of course, I don’t know that for sure.

I decided to document the fact that I knew this before anyone else. I did this in true “we-now-have-an-immutable-never-changing-database” fashion by sending myself the following Ethereum transaction:

> [0xa152016a73c308e2ecdf9ff31d6a36f717604472a589f21e998e96c3e9500344](https://etherscan.io/tx/0xa152016a73c308e2ecdf9ff31d6a36f717604472a589f21e998e96c3e9500344)

So what’s the big deal? Why does this prove anything? Click on the ‘Convert to Ascii’ button on that webpage. You will see the following message which I forever recorded on the blockchain:

> On July 11, 2016 this contract (0xb656b2a9c3b2416437a811e07466ca712f5a5b5a) which is the DAO curator contract sent a transaction to the DarkDOA (0x304a554a310c7e546dfe434669c62820b7d83490) that added the DAO’s curator to the DarkDAO’s whitelist. Here’s the transaction ID: 0x9257c2e0a11de7b7427d4607f5908d6448278070bb73500139387930826fedc0. I am writing this to the blockchain before anything happens to demonstrate the power of [http://ethslurp.com](http://ethslurp.com) software by TrueBlocks, LLC.

That was last night, July 11, 2016 at 8:58:05 PM UTC. This morning at 8:25 in the morning this tweet appeared from Stephen Taul: [https://twitter.com/stephantual/status/752886658211676160](https://twitter.com/stephantual/status/752886658211676160). I knew this 12 hours before it was announced.

#### Help Me Help You

I’ve seen a lot of commentary over the last few weeks about many different ways that the Ethereum community can make smart contracts smarter and more immune from hacks. Most of that commentary is focused (properly) on the front half of the equation — that is, the source code. There are many, many things we can do to make the source code better and protect us from bugs. But I think another interesting area that can be pursued is the learn how to better watch what happens on a contract. I’m trying to convince myself that somewhere in these ~135,000 transactions on The DAO there are some hints that we might have seen before the theft happened. I haven’t found anything yet, but you can help me by downloading my data ([http://daodeepdive.com/data.html](http://daodeepdive.com/data.html)). Try to see if you find any patterns. Help me double check that the data is correct. [Send me a tip](http://ethslurp.com/donate.html)!
