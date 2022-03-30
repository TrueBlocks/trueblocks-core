# v0.27.0 Data Correction for Unchained Index

(Mar 30, 2022)

## Why the Change?

A few days ago, we discoverd that Erigon's `devel` branch had a bug that we at TrueBlocks did not notice. During that time, our indexing scraper continued running and producing pinned data to our Unchained Index manifest. Subsequently, if you ran a command called `chifra init`, which downloads those files from IPFS, you may have gotten incorrect index files and bloom filters.

This migration corrects that invalid data. You may find that some of the data produced with the previously incorrect index was incorrect (although we expect that outcome to be very rare).

In these instructions, we assume you have a properly installed version of `chifra`, that `chifra` is in your $PATH, and the you are comfortable
on the command line.

### What do you need to do?

The migration instructs you to remove a few files and folders from your local hard drive. The migration consists of these steps:

- Find the unchained index folder on your computer
- Go to that folder and remove certain files and sub-folders
- Freshen the Unchained Index by re-running `chifra init`

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is complete.

## Instructions

### Change directory to your Unchained Index folder

Run this command:

```
chifra status --terse
```

Notice the `indexPath` value (<indexPath>). Change into that directory:

```
cd <indexPath>
tree -d .
```

You should see something similar to the following:

```
.
├── blooms
├── finalized
├── maps
├── ripe
├── staging
├── unripe
└── ts.bin
```

### Remove incorrect files and sub-folders

Make sure you're in the folder <indexPath> and that you see the above folder tree. `pwd` should report the same folder as `chifra status --terse` does for `indexPath`.

Complete the following commands:

```
rm -fR blooms/0139*
rm -fR finalized/0139*
rm -fR blooms/014*
rm -fR finalized/014*
rm -fR maps/ staging/ ripe/ unripe/
```

Run this command again:

```
tree -d .
```

you should see something like this:


```
.
├── blooms
├── finalized
└── ts.bin
```

### Freshen your index

Finally, we want to freshen your index. You may do that in one of two ways. (Both produce the same results.)

```
chifra init --all
```

```
chifra scrape indexer
```

`chifra init --all` is much faster, but requires you to trust the downloaded data. `chifra scrape` is slower, but builds the index locally from your own node, so it's presumably safer.

## You're finished!

You may now restart any processes you stopped earlier: `chifra scrape` or `chifra serve`.

Please report any problems by creating an issue.

**Upshot:** You may see slightly different results using this new index with `chifra export`. In particular, the previous index was missing appearance records, so your results may now include transactions that were not previously reported. We know of no other side effects.

## Previous Migration

[Click here](./README-v0.25.0.md) for the previous migration.
