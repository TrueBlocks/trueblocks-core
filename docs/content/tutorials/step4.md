---
title: "Step 4 - Getting Serious"
description: ""
date: 2023-09-05T19:25:12+02:00
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
---

### Step 4 - Getting Serious

In this step, we'll start to use the command line interface (CLI) to do some serious work. We'll start by looking at the `chifra` command, which is the CLI for the `chifra` package. This package is the core of TrueBlocks and is used by all the other packages. It is the package that does the heavy lifting of reading the blockchain, caching the data, and providing the data to the other packages.

```bash
chifra export --fmt json --cache trueblocks.eth
chifra export --fmt json --cache trueblocks.eth --receipts
```

```bash
chifra export --fmt json --cache trueblocks.eth --logs
chifra export --fmt json --cache trueblocks.eth --logs 0x3bb7428b25f9bdad9bd2faa4c6a7a9e5d5882657e96c1d24cc41c1d6c1910a98
chifra export --fmt json --cache trueblocks.eth --logs --emitter 0x7d655c57f71464b6f83811c55d84009cd9f5221c
```

```bash
chifra export --fmt json --cache trueblocks.eth --logs --emitter 0x7d655c57f71464b6f83811c55d84009cd9f5221c --articulate
```

```bash
chifra export --fmt json --cache unchainedindex.eth --traces
chifra export --fmt json --cache unchainedindex.eth --traces --articulate
```

```bash
chifra export --fmt json --cache --last_block 12000000 vitalik.eth --balances
chifra export --fmt json --cache --last_block 12000000 vitalik.eth --accounting --statements
```

### Some other crazy shit you can do

Every donor to TrueBlocks' GitCoin grant:

```bash
chifra export --fmt json --cache trueblocks.eth --logs --emitter 0x7d655c57f71464b6f83811c55d84009cd9f5221c --articulate | jq ".data[].articulatedLog" | jq ".inputs.dest" | sort | uniq -c | sort -n
```

The most frequent donor to TrueBlocks' GitCoin grant's most frequent donations

```bash
chifra export --fmt json --cache 0xd27d90b337717Fd91C4D409F712B890D4eE2FdD6 --logs --emitter 0x7d655c57f71464b6f83811c55d84009cd9f5221c --articulate | jq ".data[].articulatedLog" | jq ".inputs.dest" | sort | uniq -c | sort -n
```

---
**`[Show Spreadsheet]`**

---

---
[<< Prev](/tutorials/step3) | [Home](/tutorials/)
