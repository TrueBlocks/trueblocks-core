---
title: "Multi chain"
description: "Instructions for accessing multiple chains from chifra"
lead: "Instructions for accessing multiple chains from chifra."
date: 2021-10-04T10:03:01-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu: 
  docs:
    parent: prologue
weight: 0501
toc: true
---

## Accessing multiple chains

As of version 0.25.0-alpha, TrueBlocks supports accessing data from any EVM-based blockchain that supports the requisite RPC endpoints. This includes most blockchains we know of.

In order to use this new feature, you have to do almost nothing. All you have to do is add `--chain <chainname>` to any chifra command (except one or two as detailed below).

For example:

| Command                          | Notes                      |
| -------------------------------- | -------------------------- |
| chifra blocks 12                 | Default chain is mainnet   |
| chifra blocks 12 --chain mainnet | Same as default            |
| chifra blocks 12 --chain gnosis  | Block 12 from gnosis chain |

## How can this possibly work?

To be perfectly honest, it can't -- without you.

You must be running your own RPC endpoints. We built TrueBlocks in this way on purpose -- TrueBlocks is not a service. It's a locally running tool. We did this because doing otherwise promotes a world of highly centralized solutions--something we are trying very hard to avoid.

Once you do start to run your own node software, you will be very glad you did. While you can use publicly available endpoints such as Infura, you will find that you will almost certainly be rate-limited. This is not true if you run your own node.

## Support for default chains

Notwithstanding the above, we've provided default configurations for the following eight (8) EVM compatible chains:

```[bash]
mainnet, rinkeby, ropsten, goerli, kovan,
gnosis, optimism, polygon
```

In each case (except `mainnet`) we point to public RPC endpoints. It is likely that you will be rate-limited if you use these options. Providing your own endpoints is much preferred.

## The Default Chain

A new configuration item has been added to the `trueBlocks.toml` file under the `[settings]` section. That item is called `defaultChain`. The initial value is `mainnet`. After installing version 0.25.0-alpha any `chifra` command that is run without the `--chain` option will default to `defaultChain`. You may modify this and subsequently remove the `--chain` option if you wish to default to a different chain.

## Adding your own chain

To configure a chain or add a new one, you must edit an entry for the `chains` array in the `trueBlocks.toml` file.

Once added, you may use your new chain entry by naming it to the `--chain` option:

```[bash]
chifra <cmd> <options> --chain <chain-name>
```

Below is one of the pre-configured sections for Ethereum Mainnet. Following that is a description of fields making up a chain configuration. Note that the section header of the chain must start with `chains.` and becomes the name of the chain.

```[bash]
[chains.mainnet]
chainId = "1"
rpcProvider = "http://localhost:8545"
remoteExplorer = "https://etherscan.io"
apiProvider = "http://localhost:8080"
localExplorer = "http://localhost:1234"
pinGateway = "https://ipfs.unchainedindex.io/ipfs/"
symbol = "ETH"
```

Your entry must contain the following fields (some of which are optional):

| Field Name     | required | Description                                                                                        |
| -------------- | -------- | -------------------------------------------------------------------------------------------------- |
| chainId        | yes      | The chain id as returned by `eth_chainId`                                                          |
| rpcProvider    | yes      | The RPC provider for the chain. Highly preferred this is local and dedicated.                      |
| remoteExplorer |          | A remote blockchain explorer for the chain (such as EtherScan). Used by `chifra explore`           |
| pinGateway     |          | Only needed if you are running `chifra scrape --pin`. Note you will need your own Pinata endpoint. |

The following three fields are only needed if you're also running the TrueBlocks Explorer:

| Field Name    | required | Description                                                                                                  |
| ------------- | -------- | ------------------------------------------------------------------------------------------------------------ |
| apiProvider   |          | The URL given by `chifra serve` and where the explorer will get its data.                                    |
| localExplorer |          | The URL of the local explorer application. Where `chifra explore --local` will open.                         |
| symbol        |          | The symbol of the native token on the chain in question. `ETH` for mainnet, `xDAI` for `gnosis` for example. |

## What Doesn't Work?

There are two things that currently do not work with Multi-Chain:

1) The command `chifra init` only works with Ethereum Mainnet. While we will never provide an index for every chain (we must rely on our user base to do that and hopefully share it), we will, in the future be supporting some chains, notably the gnosis chain.

2) Pricing using UniSwap only works on the mainnet. In the future, once more standardization appears relative to pricing on multiple chains, this choice will be revisited.
