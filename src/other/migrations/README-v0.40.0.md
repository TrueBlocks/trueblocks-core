# v0.40.0 Fixes Unchained Index

(June 25, 2022)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding a certain bug in Erigon (or TrueBlocks, or most likely both). I thought we had protected ourselves from this problem, but I was wrong.

Somewhere between blocks 13,300,000 and 13,400,000 Erigon stopped reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records in some block ranges.

This migration corrects those errors.

## What do I need to do?

The migration consists of four steps:

1. Stop long-running processes
2. Edit the TrueBlocks configuration file
3. Run `chifra status --migrate index` to effecuate the migration
6. Restart long-running processes

Depending on the machine you're working on, this migration may take anywhere between a few minutes and a few hours.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors --watch`, or `chifra serve`). Do not restart
them until the migration is complete.

If you wish, you may make a backup copy of your unchained index folder and/or cache. Find your `cachePath` and `indexPath` with `chifra status --terse`.

## Instructions

Complete the following instructions from a folder called `$configPath`. Find the value of `$configPath` by running `chifra status --terse`.

----
### Correct Entries in trueBlocks.toml Related to the Unchained Index

Edit `$configPath/trueBlocks.toml`

1. Remove any line that contains the word `pinGateway`
2. Add the following lines to the following chain configurations:

```[toml]
[mainnet]
ipfsGateway="https://ipfs.unchainedindex.io/ipfs/"

[gnosis]
ipfsGateway="https://gnosis.unchainedindex.io/ipfs/"

[sepolia]
ipfsGateway="https://sepolia.unchainedindex.io/ipfs/"
```

3. Indicate that you've completed this part of the migration by setting the version string in the file as below. If the `[version]` section does not exist, add it.

```[toml]
[version]
current = "0.40.0-beta"
```

**Note:** If you're running your own ipfs gateway and you've pinned your own index, add the necessary values for those chains.

**Note:** A careful reader will notice that we now support indexing on three chains: `mainnet`, `gnosis`, and `sepolia`.

----
### Run the migration

Prior to completing the next part of the migration, make a backup of `$cachePath/monitors` and `$indexPath` if you wish. Find the value for those folders with `chifra status --terse`.

This part of the migration does three things:

1. Removes unneeded folders in `$indexPath` (`staging`, `unripe`, `ripe`, and `maps`)
2. Removes incorrect index and Bloom files from `$indexPath/finalized` and `$indexPath/blooms`.
3. Displays a list of any monitors that may need to be cleaned up. We do not remove these monitors, but you should. They likely contain incorrect information. Remove monitors with `chifra monitors --delete --remove <address>`.

Run

```[bash]
chifra status --migrate test
```

This command should report that you need to migrate your index folder. If it does, do this:

```[bash]
chifra status --migrate index
```

Do not quit this command. Allow it to run to the end. This step may take as much as an hour or more, but it depends on your machine. If you
stop the process mid way, you may re-run it until it again.

You will know you're finished if run `chifra status --migrate index` and it returns without doing anything.

## You're finished!

You're finished. Check to see that things worked correctly:

```[bash]
chifra chunks manifest
```

You should get valid JSON. Please contact us in our Discord if you have problems.

You may now restart any long-running processes: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

## What should I do if there's a problem

In the worst case, you may always remove the entire contents of the `$indexPath` folder, then do `chifra init`, `chifra init --all`, or `chifra scrape` to replace the index from scratch as you would with a brand new install.

If you have any other problems, please contact us in our discord.

## An important note on other chains

Prior to this migration, we "officially" supported only Ethereum mainnet. With this update we add support for the Gnosis chain
and Sepolia testnet. If you've indexed other chains, you must remove that entire index and re-build it from scratch. You may
do this by removing the contents of `$indexPath` and re-running `chifra scrape run --chain <chain>`. Find `$indexPath` with
`chifra status --terse`. We're truly sorry for the inconvienence.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
