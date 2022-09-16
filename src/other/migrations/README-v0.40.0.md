# v0.40.0 Unchained Index Version 2.0

(September 15, 2022 -- Happy Merge Day!)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044)
regarding a certain bug in Erigon (or TrueBlocks, or more likely both). We thought we had protected ourselves from this problem,
but we were wrong.

This resulted in corrupted data somewhere between blocks 13,300,000 and 13,400,000 (and elsewhere).  Erigon stopped
reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records for
some block ranges.

This migration corrects those errors, but it does [a WHOLE LOT of other things as well](../../../CHANGES.md).

## What do I need to do?

The following migration instructions apply only to Ethereum Mainnet. You may adjust for other chains by adding the `--chain` option. See the note
below if you're running against other chains.

The migration consists of the following steps:

- Stop long-running processes
- Edit configuration files
- Run and test the migration commands
- Restart long-running processes

Depending on your internet connection, the migration may take anywhere between a few minutes and a few hours. Be patient and allow the process to complete.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors`, or `chifra serve`). Do not restart them until the migration is complete.

### Make a note of some folder values

You will need the following folder locations to proceed. Get these values by running `chifra status --terse`:

1. `$configPath`
2. `$cachePath`
3. `$indexPath`

If you wish to backup these folders, you should do so now, however, please note that these are caches and can always be re-built, so this step is optional.

### Instructions

Change directory into the `$configPath`:

```
cd <configPath>
```

----
### Edit the trueBlocks.toml configuration file

Edit `$configPath/trueBlocks.toml`

1. Remove all lines in this file that contain the word `pinGateway`
2. Add the following values under the [settings] section:

```[toml]
[settings]
...
defaultGateway = "https://ipfs.unchainedindex.io/ipfs"
...
```

3. Move your Etherscan key to a new section.
  - If a key called `[settings]etherscan_key` exists, copy its value and then remove it.
  - Add three new configuration sections to the `trueBlocks.toml` file. Use the value you copied for Etherscan API key.
  - Note: The first section, [keys], is intentially empty.
  - Note: The etherscan key is suggested, but optional. The pinata keys are only needed if you intend to pin the index.

```
[keys]

[keys.etherscan]
apiKey = "<copied or new Etherscan API key>"

[keys.pinata]
apiKey = "<your Pinata api key>"       # optional
secretKey = "<your Pinata secret key>" # optional
jwt = "<your Pinata JWT key>"          # optional
```

4. At the top of the file, specify the current version. (If the `[version]` section does not exist, add it.)

```[toml]
[version]
current = "0.40.0-beta"
```

**Note:** If you're running your own ipfs gateways and pinning the index yourself, add an item called `ipfsGateway` and the appropriate keys as necessary.

**Note:** A careful reader will notice that we now index and publish indexes for three chains: `mainnet`, `gnosis`, and `sepolia`. You may add your own chains. See the documentation.

----
### Run the migration command

Make sure you're using the latest branch. Do a fresh re-build, and then run:

```
chifra chunks index --check
```

If your version of the index needs migration, it should report numerous errors. If not, you're finished.

Next, run:

```
chifra index --all            # or just `chifra init` if you're working with a minimal installation.
```

This may take a long time to run. Allow it to run to completion. If it stops, re-start it.

When that completes, run:

```
chifra chunks index --check
```

This should now work without errors. If it does, you're finished.

If this command reports errors, continue to re-run `chifra init --all` (or `chifra init`) until no errors are reported.

## What if it doesn't work

The above commands may not work for various reasons. For example, Pinata may rate-limit the `chifra init` commands. If this happens, you may run the command again. Or, add the `--sleep <n>` option to slow the process down.

If you can't get the above process to work, you can use the "nuclear option" by deleting the entire contents of `$indexPath/finalized/` and `$indexPath/blooms/`. Once you do that, running `chifra init --all` or `chifra init` (with or without `--sleep`) should work.

## Important note

If you have existing monitors (check `$cachePath/monitors`), you should remove them with `chifra monitors --delete --remove <address>` or simple delete the folder called `$cachePath/monitors`. Otherwise, those monitors may contain incorrect or missing transactions (see the original reason for this migration above).

## You're finished!

You're finished. Check to see that things worked correctly:

```[bash]
chifra chunks index --check
```

You should get valid JSON reporting no errors. Please contact us in our Discord if you have problems.

You may now restart any long-running processes: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

## What should I do if there's a problem

In the worst case, you may always remove the entire contents of the `$indexPath` folder, then do `chifra init`, `chifra init --all`, or `chifra scrape` to replace the index from scratch as you would with a brand new installation.

If you have any other problems, please contact us in our discord.

## An important note on other chains

Prior to version 0.40.0-beta, we only supported Ethereum mainnet. With this update we add support for the Gnosis chain and Sepolia testnet. If you've indexed other chains, you must remove that entire index and re-build it from scratch. (Sorry!) You may do this by removing the contents of `$indexPath/<chain>` and re-running `chifra scrape --chain <chain>`. Find `$indexPath` with `chifra status --terse`. We're truly sorry for the inconvienence. You will need to remove any monitor files as well.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
