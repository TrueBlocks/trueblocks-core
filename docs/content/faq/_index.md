---
title: "FAQ"
description: "FAQ"
lead: ""
date: 2024-02-18T13:06:08-03:00
draft: false
layout: "single"
---

{{< toc >}}

### *Usage*

#### -- Can I get my balance for a given token?

Yes. There is. In fact, this is one of TrueBlocks' most important features. Simply do `chifra export --statements <address(es)>` or query the API serve with `http://localhost:8080/export?addr=<address>&statements`.

Note that `chifra export` has many, many other options which produce similarly-informative data such as `--logs`, `--appearances`, `--neighbors`, `--accounting`, and so on. See the entire help file with `chifra export --help`.

#### -- Token four-bytes come in two flavors. How do you handle that?

[Here's an example](https://www.4byte.directory/signatures/?bytes4_signature=0xa9059cbb).

The purpose of `chifra export` is to extract all transactions necessary to do 18-decimal-place-accurate accounting for a given address (or addresses).

If we encounter such as case (where there is a conflict in the four-byte or event topic), we pull that transaction as we would any other, but...when we query for the accounting (i.e. we query the smart contract for ERC20 balances), we get either an errored response in the conflicting case. This "mistaken ERC20 transfer" will have no value. The transaction may appear in the list of all transactions, but it will have no effect on the accounting.

In other words, regular old-fashioned, off-chain double-entry accounting will comes to the rescue. This is by design because as you point out, it's not possible to be perfect using purely on-chain data.

#### -- Can I use my own ABI for the --articulate option?

The chifra abis routine will try to find the ABI in the local folder by looking for `<address>.json`, although you may specify the --sol option and feed it the solidity code. Failing that, `chifra` looks to EtherScan for the ABI. Failing that it falls back to a collection of about 2,800 'known' signatures from EIP standards (ERC20, etc.) and some popular smart contracts ENS, Zephlin, etc.

#### -- I want to make a list of all tokens I own and their historical balances. How do?

An article on why this is hard: https://trueblocks.io/blog/how-many-erc20-tokens-do-you-have/

A first draft of an article on how to accomplish this: https://github.com/TrueBlocks/tokenomics/blob/main/explorations/accounting-03/notes.md

#### -- How does chifra handle multiple accounts?

I will try to answer this myself by going through the code, but how does chifra handle multiple accounts? Say I have 10 addresses that I view as a single bundle. One wallet may buy a token and send it to another wallet that ultimately sells it. In this context, there was a buy and a sell and it doesn’t really matter which wallet it came from. Can chifra handle this situation to produce the financial statements and double entry accounting at the “bundle” level instead of the address level?
tjayrush | TrueBlocks.io — Today at 11:21 AM
Hard question to answer, but I'll try. (I might add this to the FAQ).
tjayrush | TrueBlocks.io — Today at 11:31 AM
Start at the beginnging. Everything other than ETH is an ERC20 token, so as long as they are compliant, we should be able to search for Transfer events to do perfect accounting. Unfortunately, that is not the case. Why?

Some (many?) smart contracts transfer ownership of tokens without generating an event. Most notable example of this is minting which isn't even a requirement of an ERC20 token. (Don't believe that, read the spec. It says ERC20 tokens SHOULD generate an event on mint, but they don't have to.)

Second, some ERC20 smart contracts report "accumulated earnings" based on a time span. So even though the contract's underlying ledger says a person owns X tokens, if he's held those tokens for Y days, it might report X + (rate of return * Y). And, there's no event generated each time the "balance" changes.

All that is just pre-amble, but the point is that every asset is an ERC20 token underneath it all.

chifra export --accounting --statements takes any number of addresses on the command line (use --file option to overcome command line limits). So, you can collect together a list of addresses, and run that command. Each individual address with only see tokens appear and disappear (through whatever mechanism). If one sends tokens out of one address and into another, that "transfer" will cancel out. + on one side, minus on the other. There's kind of nothing you have to do about it.

Summary: make a list of all addresses and it will just take care of itself.

Note this only works for non-CEX addresses which must be handled separately and which we don't yet have a solution.

----

### *Running a Node*

#### -- Why would anyone want to run a local Ethereum node?

It's faster, cheaper, uncensorable, and private.

**Faster:** You can hit your own RPC endpoints as fast as you could possible want. No rate limiting. It's surprsing how important this is. It transforms a "difficult-to-use node" into a perfectly fine data server.

**Cheaper:** Over time, it's way, way cheaper to run your own infrastructure. There's only a single initial capital outlay. Plus -- you don't need a huge memory machine. Once the node and our scraper are caught up to the front of the chain, there's only a single block at every so many seconds. This is easily handled for both the local node and TrueBlocks. More memory is better, of course, but is also not the main bottleneck. (In fact, there are no bottlenecks--they system is mostly waiting for new blocks from the network.)

In the past, large disc space requirements used to be a problem -- especially with an archive node such as OpenEthereum (or Geth), each of which required 10-12 TB. Erigon requires only 2-3TB for the exact same data. You need at least a 4TB hard drive, but these are increasing more available.

One could, if they wished, used "node-as-a-service" such as Infura or QuickNode, but the monthly cost is high -- up to $250.00 for a base-layer (i.e. no tracing) archive node access with 'dedicated servers' going up from there. The former suffers from the 'rate-limiting' problem, and the later is probably way more expensive. A local machine quickly pays for itself.

**Uncensorable / private:** You're running your own servers inside your own building. If someone is either censoring your data or invading your privacy, you have only yourself to blame. These two aspects of the data access should be your responsibility, not a third-party provider.

Our recommendation is definitely a local machine running Erigon, with TrueBlocks installed on the same machine. An excellent option is [dAppNode](https://dappnode.io).

#### -- The docs say you require Erigon. Is that true?

There's four reasons we suggest Erigon (the last is a deal-breaker).

1. Erigon is MUCH faster syncing -- two weeks vs many months for archive node
2. Erigon takes up MUCH less disc space - 2 TB vs. 12 TB for an archive node
3. Erigon's RPC is faster
4. Erigon natively supports the trace_ namespace. Geth supports it but only through a JavaScript emulator -- tracing is literally unusable in Geth. TrueBlocks needs tracing.

Item 1, is not that bad -- if you have the time to wait.

Item 3, is dependent on which RPC endpoints you use -- particularly tracing.

Item 2 matters immensely to us since we are so focused on running on small, decentralized machines and makes all other nodes not viable.

Item 4 is a deal breaker. Without traces, we would have to re-write the internals of our scraper. Plus, without traces, the "quality" or "completeness" of our solution is seriously compromised. We could index just events (like The Graph), but that will never allow you to reconcile (in an accounting sense) which is one of our priorities as well.

If you're running against a non-archive Geth node (or any other node), then TrueBlocks will not work very well. After all, TrueBlocks' entire purpose is to study the transactional histories of an address. Non-archive nodes do not provide any historical transactional data.

If you're running against an archive Geth node (or other node software that does not support Parity traces), again, things won't work very well. TrueBlocks requires traces to dig fully into an account's transactional history. This is not a choice of TrueBlocks, it's a choice if the node software. Without Parity traces, the node simply can't keep up with the requirement to produce an accurate index.

If you're running against node software that is both an archive node and supports Parity tracing in a performant way (such as Erigon and Nethermind), then you'll run into one more problem. Disc space usage. Geth and Nethermind (and the old OpenEthereum) take up more than 10TB of disc space if you're running an archive node. Erigon takes up 2TB. Five times less.

**Upshot:** Erigon is our greatly preferred node software. Geth is basically unsupported by TrueBlocks. Nethermind is possible, but only if you have a very large hard drive.

----

### *Building the Unchained Index*

#### -- What do the terms finalized, staging, ripe, and unripe mean?

Run this command: `chifra config`. You will see output similar to this:

```[shell]
2022/10/24 07:21:20 Client:       erigon/2022.09.3/linux-amd64/go1.18.2 (archive, tracing)
2022/10/24 07:21:20 TrueBlocks:   GHC-TrueBlocks//0.41.0-beta-20b34d9e0-20221024 (eskey, pinkey)
2022/10/24 07:21:20 RPC Provider: http://localhost:23456 - mainnet (1,1)
2022/10/24 07:21:20 Config Path:  <local path>
2022/10/24 07:21:20 Cache Path:   <local path>
2022/10/24 07:21:20 Index Path:   <local path>
2022/10/24 07:21:20 Progress:     15817943, 15817229, 15817806, 15817941, ts: 15817942
```

Notice the last line labeled "progess". What do these numbers mean? They are, in order, `latest`, `finalized`, `staged`, `ripe`, and `timestamp`. (`unripe` is not included.)

Here's what these numbers mean:

| block     | meaning                                                                            | distance<br>from head | configurable | will be<br>revisited |
| --------- | ---------------------------------------------------------------------------------- | --------------------- | ------------ | -------------------- |
| latest    | The latest block on the chain. Same as `eth_getBlockByNumber('latest').            | 0 blocks              | -            | -                    |
| finalized | The last block that has been consilidated into a "chunk". (i.e. an index portion). | depends               | **yes**      | no                   |
| staging   | The latest block "on the stage". (i.e. awaiting inclusion in the next chunk).      | depends               | **yes**      | no                   |
| ripe      | Blocks that have been scraped, but not yet staged.                                 | 28 blocks             | **yes**      | no                   |
| unripe    | Blocks that are "too close to the head" to be reliable.                            | 0 blocks              | no           | **yes**              |
| timestamp | The latest scraped timestamp (used for debugging).                                 | n/a                   | -            | no                   |

For a much better explaination of these numbers (and more generally the scraper), please see the [TrueBlocks Spec](https://trueblocks.io/papers/2022/file-format-spec-v0.40.0-beta.pdf).

#### -- I'm getting an error message: current file does not sequentially follow previous file. What do?

When using chifra scrape you may get the above message. What this means is at least one empty block was
encountered during a single pass. When I say "empty", I mean that the block did not even contain a
miner address. Our scraper assumes every block must contain at least one address, but on some chains
this is not true (for example, on some private chains).

You may turn this warning off by starting `chifra scrape` with the undocumented `--allow_missing` option. This will
disable the warning and allow the scraper to continue.

This error may also manifest itself with the message "A block was not processed."

**Important note**: On some chains, there are long stretches of such empty blocks. In this case, `--allow_missing` may
not fix the problem. `--allow_missing` works on one or a small number of missing blocks in a row, but extended ranges
of missing blocks may still cause a problem. This is related to how many blocks `chifra scrape` process in a single pass.
By default, the scraper processes 2,000 blocks at a time. If the range of empty blocks is larger than 2,000, even
settting `--allow_missing` will not help. In this case, you must increase the number of blocks processed in a pass
to be larger than the number of empty blocks in a row. Do this with the `--block_cnt` option. For example, `--block_cnt 5000`

#### -- Must I have a copy of the Unchained Index in order to use chifra?

Since TrueBlocks only provides Unchained Index data for Eth mainnet, Sepolia, and Gnosis, how can I run the `chifra init` for Polygon?

This is a very, very good question. TrueBlocks is not a "service." By that, I mean that we do not provide you (the user) with anything other than the ability to create and use the Unchained Index yourself. It's as if we were giving you a hammer as opposed to, say, being carpenter that you can hire to complete a project. We provide Eth mainnet, Sepolia, and Gnosis because *we need those chains*. If someone else needs a different chain, they need to provide it for themselves. The innovation that TrueBlocks makes is that (if it makes any), is to allow you to provide the index for yourself. Furthermore, with TrueBlocks, the index is shared with other people without doing anything special. Super importantly -- other people can share perhaps other data with you. And it flows out from there. TrueBlocks is purposefully designed this way because "decentralization," which we believe must work by default.

#### -- How do I build an index?

cryptoguru — Yesterday at 3:10 PM
I did scan the docs, but nothing really stood out to me as the way to define a sort of schema to build an index
Definitely open to any ideas you may have as input
tjayrush — Yesterday at 5:18 PM
So our software works by creating an index of what we call "appearances." An appearance is a <blockNumber.txid> pair list, for each address, where that address appears on the chain. Later, you can query that index to build a list of transactions and only then would you pull the actual transactional (or trace or log) data from the chain. You can choose to do anything you like with that data. By default, it generates either JSON, TXT, or CSV data. We don't impose any sort of database on the data.

This article describes exactly the example you mention -- pulling all UniSwap pairs: https://tjayrush.medium.com/recipe-factories-ce78fa4c5f5b

Another article discusses how to get monthly balances for a collection of accounts: https://tjayrush.medium.com/recipe-monthly-token-balances-ff6a302fda80
Medium
Recipe: Factories
Getting a list of all contracts created by an address
Medium
Recipe: Monthly Token Balances
Or, Just How Bad was my year?
cryptoguru — Yesterday at 11:29 PM
This is an excellent explanation! Thanks for sharing the blog post recipes, seems to be a great practical example with a starting point of how to make use of it for my use case. I'll take a further look and try out the examples! 

#### -- Is it normal for the index to be about six minutes behind the head?

A conversation with a user:

Hi everyone, newbie here. Is it normal for the finalized chunks on eth mainnet to be about 6 minutes behind the latest block?
Or is there a setting I can configure to help keep the finalized chunks closer to the head?
tjayrush — Today at 7:11 PM
is it normal for the finalized chunks on eth mainnet to be about 6 minutes behind the latest block?

Yes. We purposefully stay 28 blocks behind the head because the head is "unsettled." 28 is a bit arbitrary, especially since The Merge where I think finalization is way sooner.
is there a setting I can configure to help keep the finalized chunks closer to the head?

When you do chifra export, you can add --unripe to the command line. I'm not 100% sure how that works exactly. I think it will only show transactions less than 28 blocks old, but it may show all transactions. You can experiment.
This feature is "under-tested" which means it works, but not sure how well, to be honest.
Breezy — Today at 7:15 PM
Thanks @tjayrush!

My most recent finalized block is 16527791 and at the time the last finalized block on etherscan is 16530233. So mine is 2442 block behind. You're saying it should only be 28 blocks behind?
tjayrush — Today at 7:28 PM
The wording is a bit confusing. Our "finalized" blocks are older than 28 blocks and exist in two possible states. "Finalized" and "staged." Both types will no longer be considered (that is, we won't query for these blocks any longer). "Finalized" blocks are stored in chunks and pinned to IPFS. "Staged" data is "final" in the sense that we won't query it again, but it's not yet put into a chunk.

When you do chifra export (without --unripe) you get both types, so you're always running against blocks 28 blocks or older (around 6 minutes).

--unripe shows blocks less than six minutes old.

Does that help?

The Magizine Model

Magazines have exactly the same qualities as immutable data. They publish periodically. They cannot go back and correct
something previously published. They publish in chunks (issue) and collect those issues in volumes (manifests) and make
corrections via errata. And can be indexed and sliced and diced.

We should study the library science of magizine publishing. How do librarians organize magazines? How do magizines
organize themselves.

We should change our code to reflect this and suggest to the guy who's doing the EIPs that he use the magizine metaphor
over the book metaphor. Books are multi-chapter, but they are not published periodically.

We can offer Uniswap a magizine publishing model (and it should not escape our notice that magizines have subscribers).

They can:

1) provide dashboards showing all sorts of information from a raw data viewpoint -- transactions, blocks, traces, logs, neighbors, etc.
2) they can publish volumes/issues/articles/pages of downloadable data via the Unchained Index



-----------------------------------------


> Have a question: can I start using `chifra serve`  when I am in the middle of 
`chifra scrape`

 ?
dawid
 — 
Yesterday at 10:07 AM
Yes you can, but you won’t get complete data
tjayrush
 — 
Yesterday at 11:19 AM
Slight addendum. You'll get complete data for all tools other than chifra export and chifra list because only these two are dependent on the index which is being created by chifra scrape. For those two tools, you'll get as data as far along as the scraper is in its scan.



> Have a question related to chain-reorgs, when using chifra scrape - how if any reorg is handled ?

Another freaking excellent question. I have to write about this one at some point. (@Dodson, can you copy this response out somewhere and save it for later?)

We can't stay up to the very front of the chain because it changes all the time. I don't have solid numbers, but the chain reorgs many many times a day. For the reason, we consider three different types of data when scraping. We call the 'green', 'yellow', or 'red,' and just like a traffic light you can 'move forward', 'be cautious', or 'proceed at high risk' with the data.
Ro Ma
 — 
Today at 5:43 PM
I am considering to implement something related to this issue, for now I have an idea to use geth, since it has RPC notification related to reorg ( as far as I understand ). So another small store + index only for last hour of data, that knows how to handle  reorgs. Interesting what about erigon & reorgs ...
tjayrush
 — 
Today at 5:52 PM
The 'green' results of the scrape are at least 28 blocks old (about six minutes). This is an arbitrary number, but about 1.5 times longer than an article we found where Vitalik says one should wait around four minutes for 'effective finality.' Of course, it's not perfect -- the chain can revert hundreds of blocks, but we have to stop looking at some point. One reason we thing this is okay is because we do a 'reconciliation'. Which means we use regular accounting procedures to 'double check the double checking.' It's kind of the best we can do.

The 'yellow' results get put in a folder we call 'ripe' which means that the scraper will 'reconsider' the blocks one more time before moving them to the 'staging' folder. A block in the 'staging' folder is 'green', but has not yet been 'consolidated' (i.e. put into a binary chunk and published to IPFS). 'Yellow' blocks are at least six blocks old (around 1.5 minutes), but less than 28 blocks.

'Red' blocks are stored in the 'unripe' folder and are less than six blocks old. You can expect them to change without notice.

Here's a ridiculously hurried graphic that I just threw together:
Attachment file type: acrobat
Red-Yellow-Green.pdf
29.90 KB
tjayrush
 — 
Today at 6:01 PM
If you run TEST_MODE=true chifra export --help you will see some additional hidden options -- in particular, --staging and --unripe (now that I'm writing this it appears there's a missing --ripe option.

If you add no additional option chifra export will export only from the consolidated data (i.e. green and consolidated -- the chunked data). 

If you run chifra export --staging, it will output only data from the staging folder (i.e. green, but not yet consolidated).

If you run chifra export --unripe, it should output only red data, but I'm skeptical. (I think this may be a bug -- it may actually output yellow data.)

We don't have this option, but if you run chifra export --ripe it should export yellow data only.

There is no option currently to export everything. And I can see that we should have an option to export --pending, although that is not yet implemented either.

(Sorry for the long-windedness. I'm writing this in detail, so I can copy it into an issue.)

#### -- Are there commands to check the consistency of the Unchained Index?

**Answer:** `chifra chunks index --check` does some high level consistency checks (but this does require someone to have published the manifest hash to the smart contract, so maybe that doesn't work.

There's also `chifra chunks index --check --deep` which digs into the files themselves and checks that each address in the file reports true when the associated bloom filter is queried.

As far as answering the question, "Is this the exact result that one should get from building the index X?" I'm not sure that's even possible. The way "we" handled that issue is by allowing anyone to publish the hash of the manifest to the Unchained Index smart contract and then we compare the results. Another way is to by rebuilding it from scratch against different client software. We've done that twice since inception

You may also export "everything" in each chunk with `chifra chunks addresses --verbose --fmt` json but this is very, very verbose. It produces data per-chunk, so you must combine results for a single address, for example from many reports. Not optimal.

----

### *Build Problems*

#### -- What are the requirements for building and running TrueBlocks?

XXX

#### -- I am having uild problems. Can you help?

When run the make command, I got this error:

```[shell]
/data/github/trueblocks-core/src/libs/utillib/sfos.cpp:21:10: fatal error: filesystem: No such file or directory
 #include <filesystem>
compilation terminated.
libs/utillib/CMakeFiles/util.dir/build.make:902: recipe for target 'libs/utillib/CMakeFiles/util.dir/sfos.cpp.o' failed
```

**Answer:** Upgrade Ubuntu to latest version. See this https://stackoverflow.com/questions/39231363/fatal-error-filesystem-no-such-file-or-directory.

----

### *About the project*

#### -- Why does TrueBlocks use a file-based cache?

Kevin11 — Yesterday at 10:08 PM
Just curious why the TrueBlocks caches results as files?

i guess 99% of users don't have this issue but I ran out of inodes
tjayrush — Today at 8:30 AM
On a local machine, caching to a binary file is about as fast as one can get. I've always thought though that this cache could be in a database (either locally or remote). I just never got around to writing that. Mostly because the cache code is currently in C++. We're no longer writing C++ and part of our work for this year is to fully port everything to GoLang. Once we get the cache ported to GoLang, then we have a bunch of easier to implement options.
May I ask the results of chifra list <address> --count shows for the address that blow out the disc space so much so that you ran out of inodes?
(In other words, which address?)
Also, are you caching transactions or traces?
Also, are you using the --accounting options?

#### -- What's the long term vision for TrueBlocks?

**Answer:**

- 30-year vision: you can't buy a computer of any type without a blockchain node inside and that blockchain node is so well indexed, anyone can get any portion of the entire history of the world without asking permission.
- 15-year vision: a special type of node software called an indexing node that does not carry the actual details of the chain, but can build the index and share it for free.
- 5-year vision: a large number of end-user (probably desktop) application built upon an excellent, complete, automatically-shared, super-fast index.
- 1-year vision: complete the work we promised to the Ethereum Foundation as described here: [Ethereum Foundation Grant - TrueBlocks](https://docs.google.com/document/d/1HaY3UZ_Q4ZJp5zo-eoHQKBq_wlkc77R7e9wqE7jnWLg)
- 1-month vision: get a speaking gig at EthDenver.
- 1-day vision: finish porting chifra traces to GoLang.

#### -- What is your policy on new features?

**Answer:**

- New features are "sort of" on hold for now as we port the entire C++ code base to GoLang. Once that's completed, we will focus on improving speed by taking advantage of GoLang's natural concurrency. Throughout our development, as new features are requested/suggested, if the feature can be added relatively easily to the GoLang code, we may add them. If the suggested feature needs to be added to the C++ code, it probably won't be added.
