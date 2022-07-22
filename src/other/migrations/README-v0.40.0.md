- Still not working 
- On chains that have missing blocks, timestamps were incorrect 
- Doubled the speed of the scrape due to not repeating for timestamps 
- Write the hash of the PDF spec into the data files 
- Allow for pinning to local IPFS daemon and pinning service 
- Support for mainnet, sepolia, gnosis, and we've seen BSC working 
- Wrote the spec (finally) 
- New version of the smart contract allows anyone to publish 
- https://docs.ipfs.io/install/command-line/#command-line
- // TODO: BOGUS - WHAT HAPPENS TO OTHER CHAINS?
- // TODO: BOGUS - WHAT HAPPENS IF THE USER HAS CHANGED DEFAULT CHAIN

# v0.40.0 Fixes Unchained Index

(July 4, 2022)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044)
regarding a certain bug in Erigon (or TrueBlocks, or most likely both). We thought we had protected ourselves from this problem,
but we were wrong.

This resulted in corrupted data somewhere between blocks 13,300,000 and 13,400,000 (and elsewhere).  Erigon stopped
reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records for
some block ranges.

This migration corrects those errors.

These instructions apply only to Mainnet Ethereum. See the note below if you're running against other chains.

## What do I need to do?

The migration consists of the following steps:

- Stop long-running processes
- Edit a configuration file
- Run migration commands
- Test the migration
- Restart long-running processes

Depending on the machine you're on and your internet connection, the migration may take anywhere between a few minutes and a few hours.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors`, or `chifra serve`). Do not restart them until the migration is complete.

### Make a note of some folder values

You will need the following folder locations to proceed. Get these values by running `chifra status --terse`:

1. `$configPath`
2. `$cachePath`
3. `$indexPath`

Backup these folders if you wish (but note that they can be rebuilt or recovered if things go wrong, so this step is optional.)

### Instruction

Change directory into the `$configPath`: `cd <configPath>`

----
### Edit the trueBlocks.toml configuration file

Edit `$configPath/trueBlocks.toml`

1. Remove all lines that contains the word `pinGateway`
2. Add the following three lines in the suggested chain configurations:

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

Ignore the note in **trueBlocks.toml** telling you not to edit this value if it's present.

**Note:** If you're running your own ipfs gateways and you're pinning your own index, adjust the `ipfsGateway` values as necessary.

**Note:** The careful reader will notice that TrueBlocks now supports indexing three chains: `mainnet`, `gnosis`, and `sepolia`. You may add your own chains. See the documentation.

----
### Run the migration command

Prior to completing the next part of the migration, you may wish to make a backup of `$cachePath/monitors` and `$indexPath`. However, these folders are caches, so they can be re-created if something goes wrong.

This part of the migration does three things:

1. Removes temporary folders in `$indexPath` (`staging`, `unripe`, `ripe`, and `maps`)
2. Removes incorrect Index and Bloom files from `$indexPath/finalized/` and `$indexPath/blooms/`.
3. Removes incorrect values from existing monitors (see the note below).

First, confirm that you need to run the migration

```[bash]
chifra status index --migrate test
```

If you need to migrate, this command will report a back-level index file was found. If command does not report a problem, please contact us in our discord.

Run the following command to complete the migration:

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
