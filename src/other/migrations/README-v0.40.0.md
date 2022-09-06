# v0.40.0 Unchained Index Version 2.0

(September 8, 2022)

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
2. Add the following values under the indicated configuration section:

```[toml]
[settings]
...
defaultGateway = "https://[{CHAIN}].unchainedindex.io/ipfs"

[chains.mainnet]
...
ipfsGateway="https://ipfs.unchainedindex.io/ipfs/"
...
```

3. Move your Etherscan key to a new section.
   - If a key called `[settings]etherscan_key` exists, remove it (copy its value first).
   - Add three new configuration sections to the `trueBlocks.toml` file. Use the value you copied from `[settings]etherscan_key` for API key.
   - Note: the [keys] section is intentially empty.

```
[keys]

[keys.etherscan]
apiKey = <copied or new value>

[keys.pinata]
apiKey = "<your Pinata api key>"       # optional
secretKey = "<your Pinata secret key>" # optional
jwt = "<your Pinata JWT key>"          # optional
```
1. Tell the system that you've made these changes by changing the version. If the `[version]` section does not exist, add it.

```[toml]
[version]
current = "0.40.0-beta"
```

**Note:** If you're running your own ipfs gateways and you're pinning the index yourself, adjust the `ipfsGateway` values as necessary.

**Note:** The careful reader will notice that TrueBlocks now indexes and publishes the index for three chains: `mainnet`, `gnosis`, and `sepolia`. You may add your own chains. See the documentation.

----
### Run the migration command

Make sure you have the latest `master` (or `develop` if you prefer) branch. Do a fresh re-build, then run:

```
chifra chunks index --check
```

If you have a previous index, this should report a number of "errors" or suggested upgrades.

Then run:

```
chifra index --all     # or if you're working with the minimal index just `chifra init`.
```

This may take a very long time, let it run to completion. Next, re-run:

```
chifra chunks index --check
```

If this works without errors, you're finished. If this reports errors, repeatedly run `chifra init --all` and the above check until you get no more errors. Once or twice at most.

If the above process does not work, you can revert to the nuecular option by removing the entire contents of the `$indexPath` and re-running `chifra init --all`.

If you have existing monitors, you should remove and regenerate them with `chifra monitors --delete --remove <address>` or simple delete the folder called `$cachePath/monitors`.

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

Prior to version 0.40.0-beta, we only supported Ethereum mainnet. With this update we add support for the Gnosis chain and Sepolia testnet. If you've indexed other chains, you must remove that entire index and re-build it from scratch. You may do this by removing the contents of `$indexPath/<chain>` and re-running `chifra scrape run --chain <chain>`. Find `$indexPath` with `chifra status --terse`. We're truly sorry for the inconvienence. You will need to remove any monitor files as well.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
