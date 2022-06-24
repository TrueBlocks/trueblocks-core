# v0.40.0 Fixes Unchained Index

(June 25, 2022)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding a certain bug in Erigon (or TrueBlocks, or most likely both). I thought we had protected ourselves from this problem, but I was wrong.

Somewhere between blocks 13,300,000 and 13,400,000 Erigon stopped reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records in some block ranges.

This migration will correct those errors.

## A note on non-mainnet chains

Prior to this migration, the Unchained Index "officially" supported only the Ethereum mainnet. With this update we add support for the Gnosis chain and Sepolia testnet. If you've indexed other chains, you need to remove that index and
re-build it from scratch. You may do this by removing the contents of `$indexPath` and re-running `chifra scrape run --chain <chain>`.
Find `$indexPath` with `chifra status --terse`. Sorry for the inconvienence.

## What do I need to do?

The migration consists of four steps:

1. Stop long-running processes
2. Edit the TrueBlocks configuration file
3. Run `chifra status --migrate index` to effecuate the migration
6. Restart long-running processes

Depending on your internet connection and the machine you're working on, this migration will take anywhere between a few minutes and about an hour.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors --watch`, or `chifra serve`). Do not restart
them until the migration is complete.

You may wish to make a backup copy of your unchained index folder and/or cache. Find your `cachePath` and `indexPath` with `chifra status --terse`.

## Instructions

Complete these instructions from a folder called `$configPath`. Find the value of `$configPath` by
running `chifra status --terse`.

----
### Correct Entries in trueBlocks.toml Related to the Unchained Index

Edit `$configPath/trueBlocks.toml`

1. Remove any line you find that contains the word `pinGateway`
2. Add the following lines to the following chain configurations:

```[toml]
[mainnet]
ipfsGateway="https://ipfs.unchainedindex.io/ipfs/"

[gnosis]
ipfsGateway="https://gnosis.unchainedindex.io/ipfs/"

[sepolia]
ipfsGateway="https://sepolia.unchainedindex.io/ipfs/"
```
3. Indicate that you've completed the migration by altering the version stored in the file. If this section does not exist, add it.

**Note:** If you're running your own ipfs gateway and you've pinned your own index, add the necessary values for those chains.

```[toml]
[version]
current = "0.40.0-beta"
```
**Note:** A careful reader will notice that we now support indexing on three chains: `mainnet`, `gnosis`, and `sepolia`.

----
### Run the migration

Prior to completing this part of the migration, you may wish to make a backup of your existing `$cachePath/monitors` and `$indexPath` folders. Find the value for those folders with `chifra status --terse`.

The migration will do a few things:

1. Remove unneeded folders in the `$indexPath` folder (The `staging`, `unripe`, `ripe`, and `maps` folders will be removed for the `$indexPath` folder.)
2. Remove incorrect files from `$indexPath/finalized` and `$indexPath/blooms`.
3. List any monitors that need to be removed (but it will not remove them -- removing these monitors will be your choice.)

Run

```[bash]
chifra status --migrate test
```

It should report that you need to migrate your index folder. If it does, do this:

```[bash]
chifra status --migrate index
```

Allow the above command to complete. This may should not take more than an hour, but it may depending on your machine. If the
process stops mid-way, or if you quit it, you may re-run it until it completes.

You will know you're finished if `chifra status --migrate index` returns without doing anything.

## You're finished!

You're finished. Check to see that things worked correctly:

```[bash]
chifra chunks manifest
```

You should get a valid response. Please contact us in our Discord if you have problems.

You may now restart any long-running processes: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

Please report any problems by creating an issue.

## What should I do if there's a problem

In the worst case, you may always remove the entire contents of the `$indexPath` folder, then do `chifra init`, `chifra init --all`, or `chifra scrape` to replace the index from scratch.

If you have any other problems, please contact us in our discord.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
