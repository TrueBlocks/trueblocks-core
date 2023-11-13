# v2.0.0 Updated Unchained Index

(November 14, 2023)

## Automatic Migration of Config Files

The first time you run any command after downloading, building, and installing version 2.0.0 of `chifra`, you will get a message similar to the following:

```[bash]
[DATE] Your configuration files were upgraded to v2.0.0-release. Rerun your command.
```

## Not-automatic Migration of Unchained Index

The next command you run will (most likely) produce a message similar to the following (and is probably why you're at this page):

```[bash]
[DATE]
    Outdated file:  `$UNCHAINED_PATH/<chain>/blooms/000000000-000000000.bloom`
    Found version:  `trueblocks-core@v0.40.0`
    Wanted version: `trueblocks-core@v2.0.0-release`
    Error:          `incorrect header hash`

    See https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v2.0.0.md.
```

This is indicating that your existing Unchained Index is of a previous version. We're very sorry to have to indicate that you must now complete a migration. It's easy, but annoying. Sorry -- this is what happens when one is dealing with immutable, local data.

If you have gotten the above message, run this command:

```[bash]
chifra init --all --chain <chain>      # or, if you previously used `chifra init` alone flag, use that.
```

Depending on the speed of your Internet connection and the size of your chain this may take between a few minutes (`sepolia`) to as much as an hour or more (`mainnet`).

You must allow the command to compete and report sucessfully. If you quit the command (with Control+C, for example) or it fails for any reason, you must re-run the command until it completes successfully.

## Checking the migration

Run this command to check that the migration completed properly:

```[bash]
chifra chunks index --check --chain <chain>
```

You may run this repeatedly until you get a message similar to the following:

```[bash]
result   checked    visited    passed skipped failed   reason
passed     12213      12213     12213       0      0   Filenames sequential
passed      8144       8144      8144       0      0   Internally consistent
passed      4072       4072      4072       0      0   Correct version
passed     16288      16288     16288       0      0   Consistent hashes
passed      4072       4072      4072       0      0   Check file sizes
passed      8144       8144      8144       0      0   Disc files to cached manifest
passed      8144       8144      8144       0      0   Disc files to remote manifest
passed      8144       8144      8144       0      0   Remote manifest to cached manifest
```

If you do...

## You're finished

You'll know you're finished if you can run the `--check` command with no failed test.

## Migrating the Unchained Index Manually

If you've scraped your own chain, the easiest thing to do is remove the existing index and start over with `chifra scrape`.

However, if you wish not to do that, you may `--tag` your existing index with the correct version. If `withdrawals` are available for your chain, and you know the block at which `withdrawals` were enabled, you may run the following command to truncate your existing index before proceeding. If your chain does not have `withdrawals`, you may skip to the next command.

```[bash]
chifra chunks index --truncate <bn> --chain <chain>
```

The above will remove all blocks prior to `bn` from your index.

### Tagging an existing index

After doing this, run this command to `--tag` your index (and remove the above warning messages).

```[bash]
chifra chunks index --tag trueblocks-core@v2.0.0-release --chain <chain>
```

You may then run the `--check` command to verify that the migration was successful. If you have trouble, please DM us in Discord.

## Previous Migration

[Click here](./README-v0.85.0.md) for the previous migration.
