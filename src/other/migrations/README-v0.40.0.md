TODO: BOGUS

- Finish file format document schemas


- REVIEW HELP FILES
- Publish databases
- Auto publish to smart contract (on Gnosis)
- Better understand the size of the index chunks on other chains
- Remove unused pins from Pinata Gnosis, Sepolia, Old Mainnet, New Mainnet
- Download entire index from chifra init (both with and without --all) Sepolia, Gnosis, Mainnet
- HASHING IN VERSION TO INDEX
- BLAZE RETURN VALUE
- PRE_LOAD MIGRATION MESSAGE
- RENDER MANIFEST
- BLOOMS VS CHUNKS IN DOWNLOAD
- NOT SURE - SOMETHING ABOUT NOT REVISITING BLOOMS
- BLAZE QUITS EARY
- WRITING JSON MANIFEST
- NEW UNCHAINED - SOURCE OF TRUTH
- NEW UNCHAINED - CONFIG FILE
- NEW UNCHAINED - FORK BLOCKS
- NEW UNCHAINED - VERSIONS
- TESTING - SEPOLIA,GNOSIS - chifra init, chifra chunks

Some test cases showing the old bad missing data and then the new data would be good.

Issues related to this problem:

https://github.com/TrueBlocks/trueblocks-core/issues/2181
    After chifra init (with or without --all) finishes, remove files from staging, ripe, and unripe

https://github.com/TrueBlocks/trueblocks-core/issues/1998
    chifra scrape: The trouble with timestamps file and not scraping

https://github.com/TrueBlocks/trueblocks-core/issues/1574
    chifra chunks: init code

https://github.com/TrueBlocks/trueblocks-core/issues/1872
    chifra scrape: Epic Issue

Smart Contract: Automate updating the pointer in the contract

Checking: fileSize -- we should store pre-zip file size in the manifest -- won't work cross operating systems -- store os?
Checking: fileSize -- can be calcualted from internal data, I think -- at least for blooms
Checking: is the first block in the array the same as first block in range?
Checking: is the last block in the array the same as last block in range?

Checking the Manifest
----------------------
1. Does each record's endBlock equal the next record's startBlock?
2. Do the file on disc, when zipped and added to IPFS, result in the same CID stored in the manifest?
3. Is the file size of the downloaded pin the same as is in the manifest?
4. Does the list of files on Piñata agree with the list of files on the hard drive?
5. Does the list of files on Piñata agree with the list of files in the manifest?
6. Does the list of files on the hard drive agree with the list of files on Piñata?
7. Does the contents of the manifest, when added to IPFS agree with the manifest CID on Piñata?
8. Does the contents of the manifest, when added to IPFS agree with the smart contract?

Managing pins on Piñata
------------------------
1. Can I list the files on Piñata?
2. Can I download a single file from Piñata?
3. Can I upload a single file to Piñata?
4. Can I remove a file from Piñata?
5. If I do any of the above, does it agree with the manifest?

Debugging the Chunks:
----------------------
1. Can I recreate a single chunk from the blockchain directly?
2. If the newly created chunk does not agree, how can I see where the disagreement happens?
3. Can I repair a chunk on disc and/or on Piñata?

Do `grep pinGateway $TB_CONFIG`
Remove anything but ipfs.unchainedindex.io/ipfs from config file (or change the name of the value)

We need to tell them to add a section to trueBlocks for sepolia which is not included otherwise

Test -- fresh install --
    chifra init --chain sepolia ==>
    chifra chunks addresses (does this even work? -- how can it?)
    chifra export address



# v0.36.0 Fix to UnchainedIndex

(May 25, 2022)

## Why the Change

A few months ago, we made this announcement (https://discord.com/channels/570963863428661248/904527518948806686/955114745369854044) regarding trouble we were seeing with Erigon. I thought we protected ourselves from allowing incorrect data to enter the index, but I was wrong.

Somewhere between blocks 13,300,000 and 13,400,000 Erigon stopped reporting addresses correctly from it's RPC (or at least TrueBlocks stopped getting addresses).

This results in missed transactions inside that block range.

### What do you need to do?

This migration is a four step process:

1. Stop any running processes
2. Remove any incorrect index chunks from your existing index
3. Use `chifra init` to replace those missing chunks
4. Restart your processes

Depending on your internet connection, this migration will take anywhere between a few minutes and about an hour.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is complete.

You may wish to make a backup copy of your unchained index folder and/or cache. Find your `cachePath` and `indexPath` with `chifra status --terse`.

## Instructions

In the following instructions any value surrounded by <> is to be replaced with the specific value from your installation.

### Removing incorrect index chunks and bloom filters

Run this command:

```
chifra status --terse
```

Notice the `indexPath`. Change into that directory:

```
cd <indexPath>
```

Make a backup of this folder if you wish.

Next, run this command:

```
rm -fR unripe ripe staging
```

This will remove any partially indexed portions.

Next, run this:

```
rm -fR */013*
```

### Replace deleted files

You may replace deleted files with this command:

```
chifra init --all
```

Make sure to run this command. If you don't you will have an incomplete index. This is the only way to correct your index short of deleting it entirely and re-generating it from scratch.

### What should I do if there's a problem

In the worst case, you can always remove all files in the `indexPath` and re-generate the index from scratch with either

```
chifra init -all
```

or

```
chifra scrape indexer
```

The former is way faster than the latter. The latter builds the index locally from your own node. Either method should arrive at the same index.


## You're finished!

You may now restart any long-running processes you stopped earlier: `chifra scrape` or `chifra serve`.

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.32.0.md) for the previous migration.
