# v0.40.0 Fixes Unchained Index

(June 25, 2022)

## Why the Change

A few months ago, we made this announcement (https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding trouble we were seeing with Erigon. I thought we protected ourselves from allowing incorrect data to enter the index, but I was wrong.

Somewhere between blocks 13,300,000 and 13,400,000 Erigon stopped reporting addresses correctly from it's RPC (or at least TrueBlocks stopped getting addresses).

This results in missed transactions inside that block range.

## What do I need to do?

This migration is a five step process:

1. Stop any long-running processes
2. Edit the TrueBlocks configuration file
3. Remove incorrect index chunks from your existing index
4. Use `chifra init` to replace those removed files
5. Restart your long-running processes

Depending on your internet connection, this migration will take anywhere between a few minutes and about an hour.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape`, `chifra monitors --watch`, or `chifra serve`). Do not restart them until the migration is complete.

You may wish to make a backup copy of your unchained index folder and/or cache. Find your `cachePath` and `indexPath` with `chifra status --terse`.

## Instructions

Follow these instructions carefully. You can find the value of `$configPath` with `chifra status --terse`.

----
### Correct entries in trueBlocks.toml related to Unchained Index

Edit `$configPath/trueBlocks.toml`

1. Remove any line in that file that contains `pinGateway`
2. Add the following lines to the following chain configurations:

```[toml]
[mainnet]
ipfsGateway="https://ipfs.unchainedindex.io/ipfs/"

[gnosis]
ipfsGateway="https://gnosis.unchainedindex.io/ipfs/"

[sepolia]
ipfsGateway="https://sepolia.unchainedindex.io/ipfs/"
```

**Note:** If you're running your own ipfs gateway and you've pinned your own index, you may use those values above or for any other chain. TrueBlocks only indexes and pins the above chains.

----
### Replace incorrect index files with corrected index files

In the following instructions you will need a value for `$indexPath` which you can find with `chira status --terse`.

```
cd $indexPath
```

Make a backup of the contents of this folder if you wish.

List the subfolders in the `$indexPath` folder:

```[bash]
ls -l
```

You should see the following subfolders: `blooms`, `finalized`, `staging`, `ripe`, `unripe` and `maps` and a single file, `ts.bin`. DO NOT remove `ts.bin`.

In the next step, you will remove four (4) subfolders. In the next step, you will remove files in the other two folders.

Do the following from the `$indexPath` folder (`pwd` shows current folder).

First, remove unneeded folders:

```[bash]
rm -fR staging ripe unripe maps
```

Next, remove the damaged index chunks and Bloom filters.

```[bash]
rm -fR blooms/013*
rm -fR blooms/014*
rm -fR finalized/013*
rm -fR finalized/014*
```

The final step is to replace the removed files with corrected files. You may use either `chifra init` or `chifra init -all`. Use the latter command if you want both the Bloom filters and each index chunk (bigger, takes longer, faster in the long run).

Either:

```[bash]
chifra init             # if you want just Bloom filters
```

or

```[bash]
chifra init --all       # if you want Blooms and Index chunks
```

## You're finished!

You're finished. Check to see that things worked correctly:

```[bash]
chifra chunks manifest --check
```

All the tests should pass. If they don't please contact us in our Discord.

You may now restart any long-running processes you stopped earlier: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

Please report any problems by creating an issue.

## What should I do if there's a problem

In the worst case, you may always remove the entire contents of the $indexPath folder, then do `chifra init` or `chifra init --all` to replace the index from scratch.

If you have any other problems, please contact us in our discord.

## Previous Migration

[Click here](./README-v0.30.0.md) for the previous migration.
