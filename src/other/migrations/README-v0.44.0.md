# v0.44.0 Scraper Bug Fix

(September 25, 2022)

## Why the Change

In version v0.41.0 (Better Scraper), there was a bug introduced that broke `chifra scrape` when it approached the head of the chain (a difficult place to test). This release fixes that bug. So as to make sure you have a valid version of the Unchained Index, this migration is necessary.

There was second bug introduced with that release that disallowed the `--file` option with `chifra scrape`. This caused a error in the `docker` version. This release fixes that problem. If you're using the docker version, you must stop it, download and re-build the last docker code, and restart it.

There were [a number of other changes](../../../CHANGES.md) in this release.

## What do I need to do?

The following instructions are for the Ethereum Mainnet. Adjust them for other chains by adding the `--chain` option for other chains. See the note below.

The migration consists of the following steps:

- Stop long-running processes
- Complete the migration
- Restart long-running processes

The migration should take only a few minutes. Be patient and allow the process to complete.

### Before you start

Stop any long running TrueBlocks-related processes (such as the `chifra scrape`, `chifra monitors --watch`, or `chifra serve`). Do not restart them until the  migration is complete.

### A note on backups

Run `chifra status` and note these values:

- `$indexPath`
- `$cachePath`

The following process will replace or remove parts of the Unchained Index including deleting portions of the index that you may have built locally. If you wish to make a backup of these folders, you should do so now.

### Instructions

### Run the migration command

Using git, pull the latest `master` or `develop` version. Then run:

```[bash]
cd build
cmake ../src
make -j 4
```

Next, run:

```[bash]
chifra chunks index --check
```

This will likely report a few errors. If it does not, you are finished with this migration. Skip to "Your Finished."

If you do see errors, run the following command (don't ignore the comment):

```[bash]
chifra init --all

# Comment: If you've don't want to download the entire index, run `chifra init`.
```

This command may take some time. Allow it to complete. If this process stops or you quit it, you may re-run it as many times as you wish.

Run the `--check` command again:

```[bash]
chifra chunks index --check
```

There should be no errors.

If there are errors, re-run the above two commands. Continue running until there are no longer errors.

## What if it doesn't work

The above commands may not work for various reasons. For example, Pinata may rate-limit your access. If this happens, you may repeat the commands as often as you like. Add the `--sleep <n>` option to `chifra init` to slow the down process if you think you're being rate limited.

## The nuclear option

In the worst case, you may simple remove the entire contents of `$indexPath` with `chifra chunks index --truncate 0`. You will then have to re-run the scraper from scratch or re-run `chifra init -all`. Your choice.

## Important note

Given that the index was incorrect, you must remove existing monitors from the cache (otherwise, they may contain invalid records). To do this, first save a list of your existing monitors into a file:

```[bash]
chifra monitors --no_header --list | cut -f1 | tee existing
```

Delete all monitors by removing the contents of `$cachePath/monitors/`.

Using the list produced above, recreate your monitors by running `chifra list --file existing`. This should be very quick depending on how "big" the addresses you're monitoring are. Note that `chifra list` does not query the node, so it's fast, but be patient.

## You're finished!

You're finished.

You may now restart any long-running processes: `chifra scrape`, `chifra monitors --watch`, or `chifra serve`.

## What should I do if there's a problem

If you have any other problems, please contact us in our discord.

## An important note on other chains

Since version v0.40.0, we've "unofficially" supported any chain, and "officially supported" `Mainnet`, `Gnosis` and the `Sepolia` testnet. The `chifra init` commands mentioned above work only with these chains. If you've indexed other chains, use the "nuclear option" above.

## Previous Migration

[Click here](./README-v0.40.0.md) for the previous migration.
