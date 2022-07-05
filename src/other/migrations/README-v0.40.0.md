On chains that have missing blocks, timestamps were incorrect
Doubled the speed of the scrape due to not repeating for timestamps
Write the hash of the PDF spec into the data files
Allow for pinning to local IPFS daemon and pinning service
Support for mainnet, sepolia, gnosis, and we've seen BSC working
Wrote the spec (finally)
New version of the smart contract allows anyone to publish


https://docs.ipfs.io/install/command-line/#command-line



// TODO: BOGUS - WHAT HAPPENS TO OTHER CHAINS?
// TODO: BOGUS - WHAT HAPPENS IF THE USER HAS CHANGED DEFAULT CHAIN

# v0.40.0 Fixes Unchained Index

(July 4, 2022)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding a certain bug in Erigon (or TrueBlocks, or most likely both). We thought we had protected ourselves from this problem, but we were wrong.

This resulted in corrupted data somewhere between blocks 13,300,000 and 13,400,000 (and elsewhere).  Erigon stopped
reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records for
some block ranges.

This migration corrects those errors.

## What do I need to do?

The migration consists of the following steps:

1. Stop long-running processes
2. Edit a configuration file
3. Run a migration commands
4. Test the migration
5. Restart long-running processes

Depending on the machine you're on and your internet connection, the migration may take anywhere between a few minutes and a few hours.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors`, or `chifra serve`). Do not restart them until the migration is complete.

You'll need three folder vaues to complete these tasks:

1. `$configPath`
2. `$cachePath`
3. `$indexPath`

**Note:** On your disc, the above paths have chain name appended, so `$indexPath/mainnet` is what you'll find. We removed the chain name for clarity.

You can find these values by running `chifra status --terse`; however, you will have to do so with a previous version of chifra.

If you wish, make a backup of the Unchained Index folder and/or your cache. Although, these files can be easily replaced since they are caches.

These instructions are for Mainnet Ethereum. See the note below for other chains.

## Instructions

Change directories (cd) into the `$configPath`.

----
### Edit the configuration file trueBlocks.toml

Edit `$configPath/trueBlocks.toml`

1. Remove any line that contains the word `pinGateway`
2. Add the following lines in the suggested chain configurations:

```[toml]
[mainnet]
ipfsGateway="https://ipfs.unchainedindex.io/ipfs/"

[gnosis]
ipfsGateway="https://gnosis.unchainedindex.io/ipfs/"

[sepolia]
ipfsGateway="https://sepolia.unchainedindex.io/ipfs/"
```

3. Tell the system that you've completed this part of the migration by changing the version string in the TOML file. If the `[version]` section does not exist, add it.

```[toml]
[version]
current = "0.40.0-beta"
```

Ignore the note telling you not to edit this value if present.

**Note:** If you're running your own ipfs gateways and you're pinning your own index, adjust these values as necessary.

**Note:** A careful reader will notice that we now support indexing on three chains: `mainnet`, `gnosis`, and `sepolia`.

----
### Run the migration command

Prior to completing the next part of the migration, you may wish to make a backup of `$cachePath/monitors` and `$indexPath`. However, these two folders are caches, so they can be re-created.

This part of the migration requires you to run a command that does three things:

1. Removes temporary folders in `$indexPath` (`staging`, `unripe`, `ripe`, and `maps`)
2. Removes incorrect Index and Bloom files from `$indexPath/finalized` and `$indexPath/blooms`.
3. Drops incorrect values from existing monitors (see the note below).

First, let's confirm that we need to run the migration

```[bash]
chifra status index --migrate test
```

This command should report that a back-level index file was found. If it does, complete the command below. If it does not, you may need to remove your entire index a rebuild it. Contact us in our discord if this happens.

Run this command if the test reports you need to migrate:

```[bash]
chifra status index --migrate index
```

Let this command run to the end. This step may take as much as an hour or more. If, for some reason, the command stops before finishing, you should re-run it until it completes.

You will know you're finished if the command `chifra status index --migrate index` returns without doing anything.

**Note:** If you experience problems with a monitored addresses, you can always start fresh by removing it and re-running `chifra list` on that address. To remove a monitor, run `chifra monitors --delete --remove <address>`. 

**Note:** If you experience problems with these steps, you can always just remove the entire `$indexPath` folder. It will be re-built with `chifra init --all`. If you do this, also remove the `$cachePath/monitors` folder. Your monitors can be re-built as well with `chifra list`.

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

Prior to version 0.40.0-beta, we only supported Ethereum mainnet. With this update we add support for the Gnosis chain and Sepolia testnet. If you've indexed other chains, you must remove that entire index and re-build it from scratch. You may do this by removing the contents of `$indexPath` and re-running `chifra scrape run --chain <chain>`. Find `$indexPath` with `chifra status --terse`. We're truly sorry for the inconvienence. You will need to remove any monitor files as well.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
