---
title: "Step 3 - Getting Details"
description: ""
date: 2023-09-05T19:25:12+02:00
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
---

### Step 3 - Getting Details

Let's dig deeper. Let's query specific addresses. In other words, snoop.

**Note:** chances are very high that if you're not running your own node, you will get rate limted with these commands.

Do this:

```bash
chifra names GitCoin:Grants
```

This command shows about 1,900 GitCoin grants from past rounds. Add a selector to the command:

```bash
chifra names GitCoin:Grants -s 291 | tee grants.txt            # `291` is random. It just picks address that have 291 in them.
 ```

The `chifra names` tool allows you to keep track of the addresses you're interested in. Adding the `-s` option shows only the addresses and selects a subset of the addresses. The `tee` command saves the output to a file.

### Getting Balances

Now we can use this list of addresses to query the state of each of the addresses. Let's get the balance of each of the addresses.
Note how slow this is (TrueBlocks is a work in progress).

```bash
chifra state --no_header --ether --chain mainnet --file grants.txt
```

A bit slow, but this is the current ether balance of each of the grant receipients. I don't know about you, all other things equal, I'd give to a project with a smaller balance than an larger one. Even if only because they're smart enough to drain thier wallet.

Notice four other things with this command:

1. The `--no_header` option removes the header from the output. This is useful when you're piping the output into another command.
2. The `--ether` option reports the number in ether. This option is available for tools when it makes sense.
3. The `--chain mainnet` option tells the tool to use the mainnet. If you don't specify a chain, the tool will use the default chain, which is a configuration item.
4. The `--file` option tells the tool to read the list of addresses from the file `grants.txt`. This is a convenience option. You could have just as easily piped the output of the `chifra names` command into the `chifra state` command.

### Getting Token Balances

We leave this part of the tutorial as an excercise. Run `chifra tokens`. Read the help text. Figure out how to get token balances for each of the grant receipients for any token, but here's a hint:

The holdings as the latest block for each of the grants receipients for the DAI stable coin:

```bash
chifra names dai stable 0x6 -s | chifra tokens --no_header --chain mainnet 0x6b175474e89094c44da98b954eedeac495271d0f --file grants.txt --cache
```

The holdings each week for each of the grants receipients for the DAI stable coin:

```bash
chifra names dai stable 0x6 -s | chifra tokens --no_header --chain mainnet 0x6b175474e89094c44da98b954eedeac495271d0f --file grants.txt --cache 14000000-15000000:weekly
```

Don't forget you can do a server version of this as well:

```bash
curl "http://localhost:8081/tokens?addrs=0x6b175474e89094c44da98b954eedeac495271d0f&no_header&chain=mainnet&file=grants.txt&cache&blocks=14000000-15000000:weekly"

curl "http://localhost:8081/tokens?addrs=0x6b175474e89094c44da98b954eedeac495271d0f&chain=mainnet&cache&blocks=10000000-17000000:daily&fmt=txt"
```

---
**`[Talk about Tokenomics]`**

[Tokenomics](https://www.tokenomics.io/gitcoin)

---

### Getting Serious

All this is fun, but let's get serious in the next (and final) step: [Step 4 - Getting Serious](/tutorials/step4).

---
[<< Prev](/tutorials/step2) | [Next >>](/tutorials/step4) | [Home](/tutorials/)
