TELL THEM TO REMOVE THE CONTENTS OF CMAKE IF THERE'S AN ISSUE WITH CMAKE
CMake was unable to find a build program corresponding to "Ninja"
-- Using c++ standard c++17
CMake Error: CMake was unable to find a build program corresponding to "Ninja".  CMAKE_MAKE_PROGRAM is not set.  You probably need to select a different build tool.
CMake Error: CMAKE_C_COMPILER not set, after EnableLanguage
CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
-- Configuring incomplete, errors occurred!
See also "/Users/jrush/Development/trueblocks-core/build/CMakeFiles/CMakeOutput.log".
See also "/Users/jrush/Development/trueblocks-core/build/CMakeFiles/CMakeError.log".
make: *** [cmake_check_build_system] Error 1
brew install ninja on  Mac


DO NOT RUN THIS MIGRATION UNTIL INSTRUCTED

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
