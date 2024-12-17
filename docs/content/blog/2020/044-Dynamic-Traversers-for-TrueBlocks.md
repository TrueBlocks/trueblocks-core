---
title: Dynamic Traversers for TrueBlocks
lead: Customized scraping of the index
description: "Extend the chifra command line with traverses. Flexibly grab whatever data you need"
date: '2020-12-09T12:29:07.973Z'
lastmod: .lastmod
categories:
- Trueblocks
tags: ['trueblocks']
draft: false
keywords: []
weight: 956
---

## Dynamic Traversers in TrueBlocks

Ethereum nodes come to consensus on a world-wide global ledger of smart contract invocations every 14 seconds.
Everyone know this. This is what we celebrate about the chain. Not only is this data world-wide and consented-to,
but it is also permissionless. At least that’s what we’re supposed to believe.

But, is it really permissionless?

I think the answer to that question is a resounding “No.” Ask yourself how you personally get data from the
Ethereum blockchain. There’s only two answers: (1) you visit a block explorer, or (2) you go to website built
by the developers of the dApp you’re using.

You want to know what’s going on with your ENS names? You go to https://ens.domains (a website).

You want to know how your GitCoin grant is doing? You go to [http://gitcoin.co](http://gitcoin.co.) (another website).

You want to invest in DeFi? You go to Zerion (a website).

I got news for you. This is **not** permissionless.

Every time you go to a website, you are at the mercy of the human being who runs that website. In effect, you have
to say _please, oh please, great data-giver in the sky…please, please give me access to my own data._

The trouble — and the thing that makes it very obvious that this data is not permissionless — is that at any time
the server (or the human) can say “No.” They can choose to not give you your own data.

That is the opposite of permissionlessness!

In my mind, permissionlessness means you can say “Fuck you, Mr. ServerMan. It’s not possible for you to stop me from
getting the data I want, no matter what you do.”

If, in the end, blockchains are not permissionless, they are simply very expensive web servers. And if that’s the case,
then everything we’re building will be for naught.

## Using TrueBlocks to get Permissionless Data

We assume you’ve read some of our [previous articles](/blog) concerning how TrueBlocks
creates and publishes an index of address appearances. We won’t rehash that in this article.

To proceed, visit the installation instructions for downloading and installing TrueBlocks: [http://github.com/TrueBlocks/trueblocks-core](http://github.com/TrueBlocks/trueblocks-core).
Once you finished installing (if you have trouble, join our discord server), try this:

```[shell]
chifra init
```

This will initialize the TrueBlocks indexes and downloads the most recent collection of Bloom filters. Next, run
a small test to see if things are working properly

```[shell]
chifra list 0xf503017d7baf7fbc0fff7492b751025c6a78179b
```

This should return a list of about 600 appearances (`blockNumber. transaction_id` pairs. If it doesn’t, revisit the installation instructions.

Next, run this command:

```[shell]
chifra export 0xf503017d7baf7fbc0fff7492b751025c6a78179b
```

This displays the same list of transactions but, for each one, `chifra` queries the blockchain and presents the full details of each transaction to the screen. Try the above command with your own address.

There are many options to `chifra export`, all of which are presented with

```[shell]
chifra export --help
```

## Traversers

Under the hood of TrueBlocks, there is a piece of code called a Traverser. A Traverser “traverses” the list of appearances, queries the chain for transactional detail, and calls into whatever functions are associated with the Traverser.

In the `list` option, the function simply prints the `blockNumber. transaction_id` pair to the screen.

For `export` option, the display function prints the entire transaction and potentially a lot of more data such as articulation and accounting reconciliations.

Traversers are very general, and because they are C++ classes, you may customize them to do practically anything. In fact, this is what this article is about.

## Dynamic Traversers

Recently, we added an amazing new feature to TrueBlocks called “Dynamic Traversers.” The word “dynamic” has special meaning here, harkening back to an ancient computing idea called _dynamic link libraries_. A dynamic traverser is a dynamic link library that you can customize.

It’s a way of extending the `chifra` command line (and at the same time, the TrueBlocks API which mirrors the command line). This allows you to preform any task you want on any or all transactions from your accounts.

In the following paragraphs, we describe how you can write and build you own dynamic traversers.

## C++ Code

I know many of you will feel flummoxed at having to use C++. Here’s our reaction to your concerns: “Get over it! It’s so freaking worth it.”

## Example Folder

Assuming you’ve downloaded and installed TrueBlocks correctly, you may create your own Dynamic Traverser by completing these tasks. We’ll first build a simple example and then build a more complicated traverser that calculates the gas spent (and lost to error) by a given address.

From the top of the TrueBlocks repo.

```[shell]
cd ./src/examples/dynamic  
make trav1  
cp -p ./libtrav1.dylib $CONFIG/cache/obj/
```

Where $CONFIG depends on your operating system (it’s `~/Library/ Application Support/TrueBlocks` on Mac, `~/.local/share/trublocks` on Linux).

This will build a dynamic link library and copy it into the TrueBlocks’ cache where we store dynamic traversers.

Try to run the dynamic traverser

```[shell]
chifra export --load libtrav1.dylib 0xf503017d...6a78179b
```

In order to run a dynamic traverser, you first put it in the right folder and the simply tell `chifra` to load it.

This should print the hash for each transaction for the given address. Let’s take a look at the code. Open the file `trav1.cpp` in the example folder. It looks like this

![](https://miro.medium.com/max/3568/1*SnG659FoS0OA77uLeBNHnw.png)

This is the entirety of the traverser. That’s all you have to do to extract a perfect list of transaction hashes from your own accounts running straight up against your own Ethereum node. You don’t have to ask anyone for anything. Just permissionlessly take as much data as you want.

If you don’t see how powerful this — how upsetting of the web server apple cart decentralized data is — please re-read the above. Getting permissionless access to globally-agreed-to, near-perfect, world-wide data only happens if people actually have permissionless access.

## Example GasCost Traverser

Next we present another example in that same folder called `trav2`. Build it with the following commands:

```[shell]
cd ./src/examples/dynamic  
make trav2  
cp -p ./libtrav2.dylib $CONFIG/cache/objs/  
chifra export --load libtrav2.dylib 0xf503017d...6a78179b
```

This prints a table of the gas consumed for each transaction, the accumulated gas used so far, and the accumulated gas used on transactions that ended in error. We leave it as an exercise to the reader to study this fairly straight-forward code. I’ll make a few comments on the code below.

![](https://miro.medium.com/max/4336/1*49tNDX89nLCoIWvF7qdyEg.png)

Notice that the `makeTraverser` function has not changed. Only the `header` and `display` function have changed.

Also notice that the code is very simple and very short. That’s the whole beauty of these dynamic traversers. You can write them in a short afternoon and they can do amazing things. For example, you’ll notice we’re accumulating totals as we traverse the data. You could, if you wished, add subtotals once per day or once per month or whatever (notice there’s a timestamp on every transaction which makes this calculation easy).

Another thing to notice is that we’ve derived our own C++ Traverser class in this second example. This gives you infinite flexibility as far as the design of your traverser. This is the holy grail of data access to the Ethereum chain. It’s fast (because it’s fully local and not shared like a web API); it’s 100% private and completely decentralized; it’s fully customizable; it’s not a website; and it’s **permissionless**.

## Conclusion

Don’t build stuff if you don’t want to end up with the result. That’s what we always say. If you want the benefits of global permissionless data, you have to build a system that presents global permissionless data. That’s a simple straight-forward statement of fact. It’s won’t be easy, but it will be very much worth it.

If you like this article or you simply wish to support our work go to our GitCoin grant [https://gitcoin.co/grants/184/trueblocks](https://gitcoin.co/grants/184/trueblocks). Donate to the next matching round. We get the added benefit of a larger matching grant. Even small amounts have a big impact.

If you’d rather, please send ETH to our public Ethereum wallet here 0xf503017d7baf7fbc0fff7492b751025c6a78179b.

Thanks for reading.
