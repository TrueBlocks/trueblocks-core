---
title: "Step 1 - Getting Started"
description: ""
date: 2023-09-05T19:25:12+02:00
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
---

#### Getting status

Let's review the `chifra status` command. It should produce output similar to this:

```[plaintext]
Client:            Geth/v1.11.5-stable-a38f4108/linux-amd64/go1.20.2 (archive, not tracing)
TrueBlocks:        GHC-TrueBlocks//1.0.0-release (no eskey, no pinkey)
Chain Details:     sepolia (11155111/11155111)
Root Config Path:  $HOME/.local/share/trueblocks/
Chain Config Path: $HOME/.local/share/trueblocks/config/sepolia/
Cache Path:        $HOME/.local/share/trueblocks/cache/sepolia/
Index Path:        $HOME/.local/share/trueblocks/unchained/sepolia/
Progress:          4259662, 0, 0, 0 ts: 4438781
```

**A few things you'll notice:**

1. The paths listed in the middle of the output may come in handy for configuration.
2. The final line shows five numbers:
   - The first number is the lastest block on the chain.
   - The last number is the latest timestamps (this was downloaded).
   - The three middle numbers are related to the index. Ignore them for now.

#### Getting the Unchained Index

----
**`[Talk about the Unchained Index]`**
[The Unchained Index](https://docs.google.com/presentation/d/1_KoMQV7ciYctgwiGdLpwn9nU6swmlb1l-w9XQo-VrDk/edit?usp=sharing)

----

Run this command which displays the current state of the index. It will report an error.

```bash
chifra chunks blooms
```

This makes sense. It's telling you that you've not yet initalized the system. It suggests either `chifra init` or `chifra scrape`. Let's try `chifra scrape`.

Run:

```bash
chifra scrape
```

Again it complains. Here, it's complaining because you don't have a tracing node.

This is one of the reasons we tell people they want to run their own node. If you don't have a tracing node, you can still use TrueBlocks, but it will be slower and MUCH less accurate.

#### Getting the Unchained Index anyway

Let's try `chifra init`.

```bash
chifra init
```

This will download the Bloom filter part of the index from an IPFS gateway. It will take a few minutes, but when it's done, you will be able to rerun the above `chifra chunks blooms` command.

```bash
chifra chunks blooms
```

You should now see data.

#### Getting a transactional history

The whole purpose of the index is to provide an endpoint to query for transactional histories of an address. We'll come back to this, but let's try that now

```bash
chifra list trueblocks.eth
chifra list vitalik.eth
```

#### Why so many files?

Are you worried about how many files are being downloaded? Don't be. This is what the decentralized world looks like. Try syncing a Geth archive node--you'll need more that 15TB. Also, when we're done, you can simply remove the `~/.local/share/trueblocks` directory and start over.

### Conclusion

In this step, we downloaded the Bloom filter part of the Unchained Index and learned about `chifra status`.

In [Step 2 - Getting Data](/tutorials/step2), we'll explore using TrueBlocks for what it's good for. Getting Data.

---
[Next >>](/tutorials/step2) | [Home](/tutorials/)
