# v2.0.0 Updated Unchained Index

(November 14, 2023)

## Automatic Migration of Config Files

The first time you run any command after downloading, building, and installing version 2.0.0 of `chifra`, you will get a message similar to the following:

```[bash]
[DATE] Your configuration files were upgraded to v2.0.0-release. Rerun your command.
```

You may proceed without furhter ado.

## Manually Migrating the Unchained Index

The next command you run will produce a message similar to the following (and is probably why you're at this page):

```[bash]
[DATE]
    Outdated file:  `$UNCHAINED_PATH/<chain>/blooms/000000000-000000000.bloom`
    Found version:  `trueblocks-core@v0.40.0`
    Wanted version: `trueblocks-core@v2.0.0-release`
    Error:          `incorrect header version`

    See https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v2.0.0.md.
```

This is indicating that your existing Unchained Index is of a previous version. We're sorry to tell you that this requires a migration. Fear not. The migration is easy, but perhaps a little time-consuming. This is one of the downsides of immutable, local-first data.

### Step 1: Downloading an Updated Index

If you have gotten the above message, run this command:

```[bash]
chifra init --all --chain <chain> --sleep .25     # or, if you previously used `chifra init` alone flag, use that.
```

Depending on the speed of your Internet connection and the size of your chain this may take between a few minutes (for `sepolia`, for example) to as much as an hour or more (for `mainnet`).

If the chain you're seeking to migrate is not published to the Unchained Index (only `mainnet` and `sepolia` are), see below.

You must allow the command to compete and report successfully. If you quit the command (with Control+C, for example) or it fails for any reason, you must re-run the command until it completes successfully.

You may remove the `--sleep` option, but you may get rate limited.

## Checking the migration

You may run this command to check that the migration completed properly:

```[bash]
chifra chunks index --check --chain <chain>
```

Run this command (interspersed with the one above) repeatedly until you get a message similar to this:

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

If you do get this message...

## You're finished

You'll know you're finished if you can run the `--check` command with no failed test.

## Migrating the Unchained Index Manually

If you're working with a different chain or you've scraped the chain's index yourself, the easiest way to upgrade is to remove your existing index and rerun `chifra scrape`.

An alternative is `--tag` your existing index with the correct version (although this is not recommended).

If `withdrawals` are available for your chain, and you know which block `withdrawals` were enabled, you may run the following commands to first truncate your existing index and then re-scraping. If your chain does not have `withdrawals`, you may skip this step.

```[bash]
chifra chunks index --truncate <bn> --chain <chain>
```

The above will remove all blocks prior to and including the chunk containing `bn` from your index. You may then re-run `chifar scrape` to catch back up to the head.

### Tagging an existing index

After truncating, please run this command to `--tag` your index (and remove the above warning messages).

```[bash]
chifra chunks index --tag trueblocks-core@v2.0.0-release --chain <chain>
```

You may then re-run `chifra scrape` (interspersed with `chifra chunks index --check`) to rebuild your index. If you have trouble, please DM us in Discord.

## Previous Migration

[Click here](./README-v0.85.0.md) for the previous migration.
