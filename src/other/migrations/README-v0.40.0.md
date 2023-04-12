# v0.40.0 Unchained Index Version 2.0

(September 18, 2022 -- We Merged!)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044)
regarding a certain bug in Erigon (or TrueBlocks, or more likely both). We thought we had protected ourselves from this problem,
but we were wrong.

This resulted in corrupted data somewhere between blocks 13,300,000 and 13,400,000 (and elsewhere).  Erigon stopped
reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records for
some block ranges.

This migration corrects those errors, but it does [a WHOLE LOT of other things as well](../../../CHANGES.md).

## What do I need to do?

The following instructions apply only to Ethereum Mainnet. Please adjust these instructions for other chains by adding `--chain` 
option. See the note below if you're running against other chains.

The migration consists of the following steps:

- Stop long-running processes
- Edit configuration files
- Complete the migration
- Restart long-running processes

Depending on your internet connection, the migration may take between a few minutes and a few hours. Be patient and allow the process to complete.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors`, or `chifra serve`). Do not restart them until the 
migration is complete.

### Make a note of the following folders

You will need the following folder locations to proceed. Run `chifra status` and note these values:

1. `$configPath`
2. `$cachePath`
3. `$indexPath`

Note that the following process will replace or remove parts of the index on your disc including deleting index portions built by
your existing scraper that may be ahead of the manifest. If you wish to make a backup of these folders, you should do so now.

### Instructions

Change directory into the `$configPath`:

```[shell]
cd <configPath>
```

----
### Edit the trueBlocks.toml configuration file

Edit `$configPath/trueBlocks.toml`

1. Remove all lines in this file containing the word `pinGateway`
2. Add the following value under the [settings] section:

```[toml]
[settings]
...
defaultGateway = "https://ipfs.unchainedindex.io/ipfs"
...
```

3. Move your Etherscan key to a new section.

- Find a key called `[settings]etherscan_key`. Copy its value and then remove this line.
- Add three new sections to file. Use the value you copied for Etherscan API key.
- Note: The first section, [keys], is intentially left empty.
- Note: The Etherscan key is optional. The Pinata keys are needed if you intend to pin the index.

```[shell]
[keys]

[keys.etherscan]
apiKey = "<your_Etherscan_key>"

[keys.pinata]
apiKey = "<your_Pinata_api_key>"       # optional
secretKey = "<your_Pinata_secret_key>" # optional
jwt = "<your_Pinata_JWT_key>"          # optional
```

4. Go to the top of the file and update the version. (If the `[version]` section does not exist, add it.)

```[toml]
[version]
current = "v0.40.0-beta"
```

**Note:** If you're running your own IPFS gateways and pinning the index yourself, you may add the appropriate keys 
here. Also add an item called `ipfsGateway` to the appropriate chain section.

----
### Run the migration command

Make sure to build against the latest branch. Then run:

```[shell]
chifra chunks index --check
```

You may see a few error reports. Regardless, run this next command:

```[shell]
chifra init --all            # (If you've don't want all the index portions, use `chifra init` for a minimal installation.)
```

This command may take a long time. Allow it to run to completion. If it stops or you quit it, you may simply re-start it.

Run this command again:

```[shell]
chifra chunks index --check
```

You should get no errors. If you do get errors re-run the `init` command, until you no longer do.

## What if it doesn't work

The above commands may not work for various reasons. For example, Pinata may rate-limit your access. If this happens, 
you may repeat the commands. You may wish to add the `--sleep <n>` option to slow the process down.

## The nuclear option

In the worst case, you may simple remove the entire contents of `$indexPath/finalized` and `$indexPath/blooms`. Rerun the above `init` commands.

## Important note

You must remove any existing address monitors (otherwise, they may contain invalid records). You will have to re-generate these monitors.
Remove the contents of `$cachePath/monitors/`)

## You're finished!

You're finished.

You may now restart any long-running processes: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

## What should I do if there's a problem

If you have any other problems, please contact us in our discord.

## An important note on other chains

Prior to version 0.40.0-beta, we only supported Ethereum mainnet. With this update we add support for the Gnosis chain 
and Sepolia testnet. The `chifra init` commands work with those two chains, but if you've index other chains, you must start
over as per the nuclear option above.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
