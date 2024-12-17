---
title: "Step 2 - Getting Data"
description: ""
date: 2023-09-05T19:25:12+02:00
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
---

We're finally able to get some data from the chain. But are we? Let's find out.

Run this command:

```bash
chifra
```

TrueBlocks is a collection of command line tools. Notice the two groups: `Chain Data` and `Chain State`. Similar to the RPC, these tools pull data directly from the node. With a few improvements...

### Getting data

Try any of these commands:

```bash
chifra blocks 2001007
chifra transactions --fmt json 0xc9c5f47ed17445070a68fab0c5336f39e6a70a74c67c8fce2a586b5f6596b3fb
chifra receipts --fmt json 4001009.32
chifra logs --fmt json 0xc9c5f47ed17445070a68fab0c5336f39e6a70a74c67c8fce2a586b5f6596b3fb
chifra traces --fmt json 0xc9c5f47ed17445070a68fab0c5336f39e6a70a74c67c8fce2a586b5f6596b3fb
```

They bring straight, raw PRC data from the node. Try adding `--cache` to each of the above commands? What happens? Did it get faster? Why?

----
**`[Talk about the RPC endpoint]`**

----

Let's try something a bit more complicated:

```bash
chifra receipts 2010732.\* --fmt csv
```

This is every receipt (or log, or transaction, or trace if you have a tracing node) in the given block.

### Getting rate limited

How about a block with a lot of transacitons? Try this:

```bash
chifra blocks 4100000 --uniq --cache --cache_txs --cache_traces
```

----
**`[Talk about the Unchained Index]`**
[The Unchained Index](https://docs.google.com/presentation/d/1_KoMQV7ciYctgwiGdLpwn9nU6swmlb1l-w9XQo-VrDk/edit?usp=sharing)

----

**Important:** If you get rate limited, don't run the command again. They may cut you off permenantly.

Did you get rate limited? We didn't.

### **Lesson**

Decentralizing your data source brings a number of massive upsides to you.

1. There is no arbitrary rate limit to the data. You're not sharing.
2. The API provider cannot be spying on you. There is no API provider.
3. With greater speed, comes greater insight into what's going on.

No matter what Web 2.0 API providers do, they MUST rate limit you. If they didn't, they would be swamped with requests. Plus -- price. Plus -- privacy. Why wait?

### Yeah. But I really want an API

Every chifra command is available through a local API server. Try this:

In one window:

```bash
chifra serve
```

In another window:

```bash
curl "http://localhost:8080/blocks?blocks=2001007"
```

<!-- ### Getting fresh data

Chifra has a command called `chifra monitors --watch` that will watch the chain for new appearances of a list of addresses. This could be the basis for a real-time notification system and/or dashboard.

```bash
chifra monitors --list
echo "chifra export" >commands.fil
cat grants.txt
chifra monitors --watch --commands commands.fil --addresses grants.txt --sleep 1
``` -->

### Getting names / abis / dates

Chifra provides a lot other very useful tools. Check them out.

```bash
chifra names
chifra names vitalik.eth
chifra names unchainedindex.eth
chifra when 2001001                                                       # by block
<!-- chifra when 1664656944                                                    # by timestmap
chifra when 2022-10-01T20:42:24                                           # by date -->
chifra when 2000000-4000000:weekly                                        # time period ranges
chifra abis --chain mainnet 0x6b175474e89094c44da98b954eedeac495271d0f    # requires an Etherscan key
```

### What's next?

In the next step, [Step 3 - Getting Details](/tutorials/step3) we're going to look a bit deeper.

---
[<< Prev](/tutorials/step1) | [Next >>](/tutorials/step3) | [Home](/tutorials/)
