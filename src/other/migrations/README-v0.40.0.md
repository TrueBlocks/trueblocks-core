# v0.40.0 Fixes Unchained Index

(June 25, 2022)

## Why the Change

A few months ago, we made [this announcement](https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding a certain bug in Erigon (or TrueBlocks, or most likely both). I thought we had protected ourselves from this problem, but I was wrong.

Somewhere between blocks 13,300,000 and 13,400,000 Erigon stopped reporting addresses accurately (and TrueBlocks didn't notice). This resulted in missing appearance records in that block range.

This migration will correct those errors.

## What do I need to do?

The migration consists of five steps:

1. Stop long-running processes
2. Edit the primary TrueBlocks configuration file
3. Remove incorrect index chunks from your hard drive
4. Use `chifra init` to replace those removed files
5. Restart long-running processes

Depending on your internet connection and the machine you're working on, this migration will take
anywhere between a few minutes and about an hour.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors --watch`, or `chifra serve`). Do not restart
them until the migration is complete.

You may wish to make a backup copy of your unchained index folder and/or cache. Find your `cachePath` and `indexPath` with `chifra status --terse`.

## Instructions

You must complete these instructions in a certain folder called `$configPath`. You can find the value of `$configPath` by
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

**Note:** If you're running your own ipfs gateway and you've pinned your own index, you may add those values above or for any other chain.

**Note:** A careful reader will notice that we now support indexing on three chains: `mainnet`, `gnosis`, and `sepolia`.

----
### Replace incorrect index files with corrected index files

Having corrected the configuration file, you now need to remove the index portions that are in error. Here, you need a value for `$indexPath` which may also find with `chira status --terse`.

Change into that directory. (Make a backup of the contents of this folder if you wish.)

```
cd $indexPath
```

Make sure you are where you think you are:

```[bash]
pwd
```

The current working path should end with `unchained`.

List the contents of the `$indexPath` folder:

```[bash]
ls -l
```

You should see the following subfolders `blooms`, `finalized`, `staging`, `ripe`, `unripe` and `maps` and a single file called `ts.bin`.

==> DO NOT remove the file `ts.bin`.

### Remove unneeded folders and incorrect files

In the next step, you will remove four (4) subfolders. In the step after that, you will remove certain files in the remaining folders.

Do the following from the `$indexPath` folder.

Remove unneeded folders:

```[bash]
rm -fR staging ripe unripe maps
```

Remove the damaged index chunks and Bloom filters.

```[bash]
rm -fR blooms/013*
rm -fR blooms/014*
rm -fR blooms/015*
rm -fR finalized/013*
rm -fR finalized/014*
rm -fR finalized/015*
```

(This is basically removing all files after block 13,000,000 which is near where the error occured.)

### Replace the removed files

The final step is to replace the removed files. You may use either `chifra init`, `chifra init -all`, or `chifra scrape` depending on your preference. Use `chifra init -all` or `chifra scrape` if you want both the Bloom filters and each index chunk. Use `scrape` if you wish to create the files yourself as opposed to downloading them.

Either:

```[bash]
chifra init             # to download only Bloom filters
```

or

```[bash]
chifra init --all       # to download Blooms and Index chunks
```

or

```[bash]
chifra scrape           # to build Blooms and index yourself
```

Allow all of the above commands to run to completion. If the process stops, or you quit it, re-run it until it completes.

## You're finished!

You're finished. Check to see that things worked correctly:

```[bash]
chifra chunks manifest --check
```

All the tests should pass. If they don't please contact us in our Discord.

You may now restart any long-running processes you stopped earlier: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

Please report any problems by creating an issue.

## What should I do if there's a problem

In the worst case, you may always remove the entire contents of the `$indexPath` folder, then do `chifra init`, `chifra init --all`, or `chifra scrape` to replace the index from scratch.

If you have any other problems, please contact us in our discord.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
