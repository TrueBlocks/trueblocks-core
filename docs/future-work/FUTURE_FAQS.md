# A list of future FAQs

## FAQ 1

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

## FAQ 2

Kevin11 — Yesterday at 10:08 PM
Just curious why the TrueBlocks caches results as files?

i guess 99% of users don't have this issue but I ran out of inodes
tjayrush — Today at 8:30 AM
On a local machine, caching to a binary file is about as fast as one can get. I've always thought though that this cache could be in a database (either locally or remote). I just never got around to writing that. Mostly because the cache code is currently in C++. We're no longer writing C++ and part of our work for this year is to fully port everything to GoLang. Once we get the cache ported to GoLang, then we have a bunch of easier to implement options.
May I ask the results of chifra list <address> --count shows for the address that blow out the disc space so much so that you ran out of inodes?
(In other words, which address?)
Also, are you caching transactions or traces?
Also, are you using the --accounting options?

## FAQ 3

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

## FAQ 4

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
