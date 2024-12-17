---
title: "Indexing Addresses"
description: "Using TrueBlocks to Create the Missing Index"
date: 2020-11-16T13:59:39+01:00
lastmod: 2020-11-16T13:59:39+01:00
draft: true
images: []
weight: 0500
toc: true
---

This article assumes that you've already [Installed TrueBlocks core](/docs/prologue/installing/). If you haven't, please do so before proceeding.

## Introduction

TrueBlocks builds an index of "every appearance of every address anywhere on the chain". With this index, we can build fully-local, 100% decentralized dApps such as the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer).

In this article, we explain how you can get a copy of the TrueBlock index of appearances. We won't explain some of the engineering decisions we've made as that is not the point of this article. Please see our [Blog](/blog/) for that type of information.

## Building the Index

If you have access to an Ethereum tracing node such as TurboGeth, you can build the TrueBlocks index yourself. For information on that, please see our [Blog](/blog/).

For this page, we take advantage of the fact that TrueBlocks, LLC (the company) produces the index and publishes it to IPFS. We do this for our own reasons─our software doesn't work without it.
We purposefully publish the index data to IPFS so that once our users have it, we can not take it back. This makes it impossible for us to hold our users hostage.

Each time we publish an index chunk (and its associated Bloom filter) to IPFS, we add a record to a _manifest_. The manifest lists all of the index chunks and Bloom filters.

So we can't take it back, we publish the manifest to IPFS. This IPFS hash to the manifest gives us a list of all the IPFS hashes of the entire address index.
In other words, we've published immutable, irrevocable access to the entire index for anyone who has the hash of the manifest to use─not only now but into the far foggy future.
The manifest will be there forever. It's immutable data and it's permissionlessly, globally available.

We go one step further─we publish the hash of the manifest to a smart contract called [The Unchained Index](http://unchainedindex.io).

## Getting the Latest Manifest

You can use `chifra` to retrieve the location of the manifest file from the Unchained Index. Let's look at `chifra state`.

## Chifra Names, Chifra Abis, Chifra State

`chifra` allows you to do many things. We will use `chifra` to accomplish the task at hand which is to get the location of the manifest of the TrueBlocks address index.

The first command we need is `chifra names`.

## Chifra Names

One of the things `chifra` needs is the ability to attach Human understandable names to Ethereum addresses. Type

```shell
chifra
```

You can see that one of the commands is `names`. Type

```shell
chifra names --help
```

This produces this help text

```shell
Usage:    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] <term> [term...]
Purpose:  Query addresses and/or names of well known accounts.

Where:
  terms                 a space separated list of search terms (required)
  -e  (--expand)        expand search to include all fields
  -m  (--match_case)    do case-sensitive search
  -c  (--custom)        include your custom named accounts
  -p  (--prefund)       include prefund accounts
  -n  (--named)         include well know token and airdrop addresses in the search
  -a  (--addr)          display only addresses in the results
  -s  (--entities)      display entity data
  -g  (--tags)          export the list of tags and subtags only
```

You can type `chifra names` to see all the addresses we've accumulated over the years. (Yes─we can use ENS to enhance this search and we plan to in the near future.)

Type

```shell
chifra names Unchained Index
```

This should return the following address

```shell
0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
```

Copy and paste this response. We will need it. The next thing we need is the ABI for that address. Do this

```shell
chifra abis 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
```

This returns the full ABI of the Unchained Index smart contract. You should be able to see a function called `manifestHash` with a four-byte signature of `0x337f3f32`.

Now we're going to do something fun. Type this

```shell
chifra state --call "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd!0x337f3f32"
```

You should get some interesting JSON data with one field called `manifestHash` equal to an IPFS hash.

What happened here? TrueBlocks queried the smart contract called Unchained Index to retrieve the value stored in the `manifestHash` variable. We can now use that to get the manifest of the entire TrueBlocks address index.

Use this command to retrieve the manifest from IPFS:

```shell
curl -s "http://gateway.ipfs.io/ipfs/<manifestHash>" -o manifest.txt
```

If you look inside that file, you will see row upon row of three fields

```shell
block-range, bloom hash, index chunk hash
012356406-012358738, QmSJeyXsvNpyXprdfwL5JyiS39VLU7m1kQNun4uM5XQbqu, QmXAZCtJ89KWWMLUkEMoTSkGSiai6aAut9kfYuWBxEoxhS
```

You can retrieve the bloom thus

```shell
curl -s "http://gateway.ipfs.io/ipfs/QmSJeyXsvNpyXprdfwL5JyiS39VLU7m1kQNun4uM5XQbqu" -o 012356406-012358738.bloom
```

## Getting all the Blooms

Additionally, we publish the IPFS hash of a manifest file to smart contract we call the _Unchained Index._ The manifest file details all the hashes of all the chunks of the index along with all the Bloom filters.

[ UNDER CONSTRUCTION -- PLEASE RETURN LATER ]
